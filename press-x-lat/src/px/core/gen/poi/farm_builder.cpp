// name: farm_builder.cpp
// type: c++
// desc: class methods implementation
// auth: is0urce

#include "farm_builder.hpp"

#include <px/common/rectangle.hpp>
#include <px/fn/bsp.hpp>

#include <px/core/gen/build_result.hpp>
#include <px/core/gen/builder.hpp>

#include <random>
#include <tuple>

namespace px
{
	namespace core
	{
		namespace
		{
			static const unsigned int sector_size = 15;
			static const unsigned int room_size = 4;
			static const unsigned int wall_size = 1;
			typedef std::mt19937 rng_type;

			enum class farm_sector : int
			{
				house = 0,
				field = 1,
				barn = 2,
				garden = 3,

				min_value = house,
				max_value = garden
			};

			enum class field_variant : int
			{
				aligned,
				unaligned,
				fenced,

				min_value = aligned,
				max_value = fenced
			};

			rectangle shrink_line(rectangle const& line)
			{
				if (line.range().x() > line.range().y())
				{
					return rectangle(line.start().moved_axis<0>(1), line.range().moved_axis<0>(-2));
				}
				else
				{
					return rectangle(line.start().moved_axis<1>(1), line.range().moved_axis<1>(-2));
				}
			}
			template <typename Generator, typename Item>
			Item const& random_item(std::vector<Item> const& vec, Generator &rng)
			{
				if (vec.empty()) throw std::runtime_error("px::random_item(vector<i> v, generator rng) - vector size = 0");
				return vec[std::uniform_int_distribution<std::vector<Item>::size_type>(0, vec.size() - 1)(rng)];
			}
			template <typename Generator>
			point2 random_range(point2 const& range, Generator &rng)
			{
				return point2(std::uniform_int_distribution<int>(0, range.x())(rng), std::uniform_int_distribution<int>(0, range.y())(rng));
			}
			template <typename Generator>
			point2 random_rectangle_point(rectangle const& rect, Generator &rng)
			{
				return rect.start() + point2(std::uniform_int_distribution<int>(0, rect.range().x() - 1)(rng), std::uniform_int_distribution<int>(0, rect.range().y() - 1)(rng));
			}

			rng_type generator(unsigned int seed)
			{
				return rng_type(seed);
			}

			template <typename E, typename Generator>
			E random_enum(Generator &rng)
			{
				int min = static_cast<int>(E::min_value);
				int max = static_cast<int>(E::max_value);
				return static_cast<E>(std::uniform_int_distribution<int>{min, max}(rng));
			}
		}

		farm_builder::farm_builder() {}
		farm_builder::~farm_builder() {}

		void farm_builder::build(unsigned int seed, rectangle const& bounds, build_result &result) const
		{
			rng_type rng(seed);
			rng.discard(rng_type::state_size);

			bounds.enumerate([&](point2 const& location) {
				result.tiles[location] = build_tile::no_change;
			});

			fn::bsp<rng_type>::enumerate(rng, bounds, sector_size, [&](auto const& sector) {
				auto sector_bounds = sector.bounds.deflated(1); // some space between "districts"
				farm_sector index = random_enum<farm_sector>(rng);
				switch (index)
				{
				case farm_sector::house:
					{
						point2 shrink = random_range(sector_bounds.range() / 4, rng);
						auto house_bounds = rectangle(sector_bounds.start() + random_range(shrink, rng), sector_bounds.range() - shrink);
						build_house(rng(), house_bounds, result);
					}
					break;
				case farm_sector::barn:
					build_field(rng(), sector_bounds, result);
					break;
				case farm_sector::field:
					build_field(rng(), sector_bounds, result);
					break;
				case farm_sector::garden: // no change, keep trees
					break;
				}
			});
		} // build()

		void farm_builder::build_field(unsigned int seed, rectangle const& sector_bounds, build_result &result) const
		{
			auto rng = generator(seed);

			switch (random_enum<field_variant>(rng))
			{
			case field_variant::aligned:
				sector_bounds.enumerate([&](auto const& location) {
					result.tiles[location] = build_tile::soil;
				});
				break;
			case field_variant::unaligned:
			{
				point2 shrink = random_range(sector_bounds.range() / 4, rng);
				auto field_bounds = rectangle(sector_bounds.start() + random_range(shrink, rng), sector_bounds.range() - shrink);
				field_bounds.enumerate([&](auto const& location) {
					result.tiles[location] = build_tile::soil;
				});
			}
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

		void farm_builder::build_house(unsigned int seed, rectangle const& house_bounds, build_result &result) const
		{
			auto rng = generator(seed);
			fn::bsp<rng_type> room_bsp(rng, house_bounds, room_size, wall_size); // room_zise rooms with 1-tile wall

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
				auto doorway = shrink_line(std::get<0>(tangle).inflated(wall_size) & std::get<1>(tangle).inflated(wall_size));
				bool last = untangled.size() == 1;

				switch (last ? 0 : std::uniform_int_distribution<int>(0, 1)(rng)) // 0 - door, 1 - random size gap
				{
				case 0:
				case 1:
				default:
					result.tiles[random_rectangle_point(doorway, rng)] = build_tile::door_ark;
					break;
				}

				untangled.erase(std::remove(untangled.begin(), untangled.end(), std::get<0>(tangle)), untangled.end());
				tangled.push_back(std::get<0>(tangle));
			}

			// add habitants
			room_bsp.enumerate_bounds([&](auto const& room) {

				result.placeables.emplace_back(random_rectangle_point(room, rng), build_placeable::mobile);
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
					result.placeables.emplace_back(center, build_placeable::table);
				}
			}); // build_house()
		}
	}
}