// name: build_housing.hpp
// type: c++
// desc: template functions
// auth: is0urce

#include <px/common/rectangle.hpp>

#include <px/fn/bsp.hpp>
#include <px/fn/build_aux.hpp>
#include <px/fn/build_result.hpp>

#include <tuple>

namespace px
{
	namespace fn
	{
		namespace
		{
			static const unsigned int wall_size = 1; // sugar for formulaes

			enum class field_variant : int
			{
				aligned,
				unaligned,
				fenced,

				min_value = aligned,
				max_value = fenced
			};
		}

		// crops field
		template<typename Generator>
		void build_field(rectangle const& sector_bounds, build_result &result, Generator &rng)
		{
			switch (random_enum<field_variant>(rng))
			{
			case field_variant::aligned:
				sector_bounds.enumerate([&](auto const& location) {
					result.tiles[location] = build_tile::soil;
				});
				break;
			case field_variant::unaligned:
				random_rectangle<4>(sector_bounds, rng).enumerate([&](auto const& location) {
					result.tiles[location] = build_tile::soil;
				});
				break;
			case field_variant::fenced:
			{
				bool fence = std::uniform_int_distribution<int>{ 0, 1 }(rng) == 0;
				int i = 0;
				sector_bounds.enumerate_border([&](auto const& location) {
					if (i == 0)
					{
						i = std::uniform_int_distribution<int>{ 3, 9 }(rng);
						fence = !fence;
					}
					--i;
					result.tiles[location] = fence ? build_tile::wall : build_tile::no_change;
				});

				sector_bounds.deflated(2).enumerate([&](auto const& location) {
					result.tiles[location] = build_tile::soil;
				});
			}
			break;
			}
		}

		// barn for animals
		template<typename Generator>
		void build_barn(rectangle const& bounds, build_result &result, Generator &rng)
		{
			auto barn_bounds = random_rectangle<4>(bounds, rng);
			barn_bounds.enumerate([&](auto const& location) {
				result.tiles[location] = build_tile::wall;
			});
			auto inside_bounds = barn_bounds.deflated(wall_size);

			// select axis to split
			bool horisontal = std::uniform_int_distribution<int>{ 0, 1 }(rng) == 0;
			int w = inside_bounds.range().x();
			int h = inside_bounds.range().y();
			if (w * 3 >= h * 4)
			{
				horisontal = true;
			}
			else if (h * 3 > w * 4)
			{
				horisontal = false;
			}

			int min = (horisontal ? w / 4 : h / 4);
			int max = (horisontal ? w : h) - min - wall_size;

			rectangle inside;
			rectangle outside;

			int c = std::uniform_int_distribution<int>{ min, max }(rng);

			if (horisontal)
			{
				inside = { inside_bounds.start(),{ c , h } };
				outside = { inside_bounds.start().moved(point2(c + wall_size, 0)), point2(w - c - wall_size, h) };
			}
			else
			{
				inside = { inside_bounds.start(),{ w , c } };
				outside = { inside_bounds.start().moved(point2(0, c + wall_size)), point2(w, h - c - wall_size) };
			}

			if (std::uniform_int_distribution<int>{ 0, 1 }(rng) == 0)
			{
				std::swap(inside, outside);
			}

			inside.enumerate([&](auto const& location) {
				result.tiles[location] = build_tile::floor;
			});
			outside.enumerate([&](auto const& location) {
				result.tiles[location] = build_tile::no_change;
			});

			// inside door
			auto doorway = shrink_ark(inside.inflated(wall_size) & outside.inflated(wall_size));
			result.tiles[random_point(doorway, rng)] = build_tile::door_ark;

			// outside door
			std::vector<point2> entrance_door_candidates;
			entrance_door_candidates.reserve(outside.perimeter());
			outside.inflated(wall_size).enumerate_border([&](auto const& location) {
				if (barn_bounds.is_border(location) && !barn_bounds.is_corner(location))
				{
					entrance_door_candidates.push_back(location);
				}
			});
			auto entrance_door = random_item(entrance_door_candidates, rng);
			result.tiles[entrance_door] = build_tile::door_ark;

			// placeables
			inside.enumerate_border([&](auto const& location)
			{
				if (std::uniform_int_distribution<unsigned int>{1, 100}(rng) <= 10 && !result.exists(location))
				{
					result.placeables.emplace_back(location, build_placeable::furniture_barrel);
				}
			});
			outside.enumerate_border([&](auto const& location)
			{
				if (std::uniform_int_distribution<unsigned int>{1, 100}(rng) <= 5 && !result.exists(location))
				{
					result.placeables.emplace_back(location, build_placeable::furniture_barrel);
				}
			});

			// add habitants
			result.placeables.emplace_back(random_point(inside, rng), build_placeable::animal_domestic);
			result.placeables.emplace_back(random_point(outside, rng), build_placeable::animal_domestic);
		}

		// housing
		template<typename Generator>
		void build_house(unsigned int room_size, rectangle const& house_bounds, build_result &result, Generator &rng)
		{
			bsp<Generator> room_bsp(rng, house_bounds, room_size, wall_size); // room_zise rooms with 1-tile wall

			// initial fill with walls and floor
			house_bounds.enumerate([&](auto const& location) {
				result.tiles[location] = build_tile::wall;
			});
			room_bsp.enumerate_bounds([&](auto const& room) {
				room.enumerate([&](point2 const& location) {
					result.tiles[location] = build_tile::floor;
				});
			});

			// detect entrance door candidates
			std::vector<rectangle> entrance_room_candidates;
			entrance_room_candidates.reserve(room_bsp.count()); // most rooms at border anyway
			room_bsp.enumerate_bounds([&](auto const& room) {
				rectangle room_inflated = room.inflated(wall_size + 1);
				if ((room_inflated & house_bounds) != room_inflated) // touching outside wall
				{
					entrance_room_candidates.push_back(room);
				}
			});

			// select entrance door
			auto entrance_room = random_item(entrance_room_candidates, rng);
			std::vector<point2> entrance_door_candidates;
			entrance_door_candidates.reserve(entrance_room.perimeter());
			auto room_inflated = entrance_room.inflated(wall_size);
			room_inflated.enumerate_border([&](auto const& location)
			{
				if (house_bounds.is_border(location) && !room_inflated.is_corner(location)) entrance_door_candidates.push_back(location);
			});
			auto entrance_door = random_item(entrance_door_candidates, rng);
			result.tiles[entrance_door] = build_tile::door_ark;

			// interconnect rooms
			std::vector<rectangle> untangled;
			std::vector<rectangle> tangled;
			untangled.reserve(room_bsp.count());
			tangled.reserve(room_bsp.count());
			room_bsp.enumerate_bounds([&](auto const& room) {
				if (room == entrance_room)
				{
					tangled.push_back(room);
				}
				else
				{
					untangled.push_back(room);
				}
			});
			while (!untangled.empty())
			{
				std::vector<std::tuple<rectangle, rectangle>> tangle_candidates;
				for (auto const& untangled_room : untangled)
				{
					for (auto const& tangled_room : tangled)
					{
						rectangle cross = untangled_room.inflated(wall_size) & tangled_room.inflated(wall_size);
						if (!cross.empty() && cross.size() >= 3) // not empty and not corners
						{
							tangle_candidates.emplace_back(untangled_room, tangled_room);
						}
					}
				}

				auto tangle = random_item(tangle_candidates, rng);
				auto doorway = shrink_ark(std::get<0>(tangle).inflated(wall_size) & std::get<1>(tangle).inflated(wall_size));
				bool last = untangled.size() == 1;

				switch (last ? 0 : std::uniform_int_distribution<int>(0, 1)(rng)) // 0 - door, 1 - random size gap
				{
				case 0:
				case 1:
				default:
					result.tiles[random_point(doorway, rng)] = build_tile::door_ark;
					break;
				}

				untangled.erase(std::remove(untangled.begin(), untangled.end(), std::get<0>(tangle)), untangled.end());
				tangled.push_back(std::get<0>(tangle));
			}

			// add habitants
			room_bsp.enumerate_bounds([&](auto const& room) {

				result.placeables.emplace_back(random_point(room, rng), build_placeable::mobile);
				room.enumerate_border([&](auto const& location)
				{
					if (std::uniform_int_distribution<unsigned int>{1, 100}(rng) <= 15 && !result.exists(location))
					{
						result.placeables.emplace_back(location, build_placeable::furniture);
					}
				});
				auto center = room.start() + room.range() / 2;
				if (!result.exists(center))
				{
					result.placeables.emplace_back(center, build_placeable::furniture_table);
				}
			});
		}
	}
}