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
			static const unsigned int passage_size = wall_size + 1; // require empty tiles without furniture in front of doors
			static const double placeables_density = 0.15;

			enum class field_variant : int
			{
				aligned,
				unaligned,
				fenced,

				min_value = aligned,
				max_value = fenced
			};

			// deflate horisontaly or verticaly (wichever is longer)
			rectangle shrink_ark(rectangle const& line, int size) noexcept
			{
				if (line.horisontal())
				{
					return line.deflated_axis<0>(size);
				}
				else
				{
					return line.deflated_axis<1>(size);
				}
			}
			// inflate horisontaly or verticaly (wichever corresponding 'line' argument is shorter)
			rectangle inflate_passage(rectangle const& door, rectangle const& line, int size) noexcept
			{
				if (line.horisontal())
				{
					return door.inflated_axis<1>(size);
				}
				else
				{
					return door.inflated_axis<0>(size);
				}
			}
			bool place_available(std::vector<rectangle> const& rectangles, build_result &result, point2 point)
			{
				return !result.exists(point) && !std::any_of(std::begin(rectangles), std::end(rectangles), [point](auto const& rect) { return rect.contains(point); });
			}
		}

		// crops field
		template<typename Generator>
		void build_fence(rectangle const& sector_bounds, build_tile tile, build_result &result, Generator &rng)
		{
			bool fence = std::uniform_int_distribution<int>{ 0, 1 }(rng) == 0;
			int i = 0;
			sector_bounds.enumerate_border([&](point2 const& location) {
				if (i == 0)
				{
					i = std::uniform_int_distribution<int>{ 3, 9 }(rng);
					fence = !fence;
				}
				--i;
				if (fence)
				{
					result.tiles[location] = tile;
				}
			});
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
				build_fence(sector_bounds, build_tile::wall, result, rng);
				sector_bounds.deflated(2).enumerate([&](auto const& location) {
					result.tiles[location] = build_tile::soil;
				});
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
			auto doorway = shrink_ark(inside.inflated(wall_size) & outside.inflated(wall_size), wall_size);
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
			point2 spawn;
			spawn = random_point(inside, rng);
			if (!result.exists(spawn))
			{
				result.placeables.emplace_back(spawn, build_placeable::animal_domestic);
			}
			spawn = random_point(outside, rng);
			if (!result.exists(spawn))
			{
				result.placeables.emplace_back(spawn, build_placeable::animal_domestic);
			}
		}

		// housing
		template<typename Generator>
		void build_house(unsigned int room_size, rectangle const& house_bounds, build_result &result, Generator &rng)
		{
			std::vector<rectangle> rooms;
			std::vector<rectangle> passages; // room for movement

			build_house_rooms(house_bounds, room_size, rooms, rng); // remove random corners to make house less square

			// initial fill with walls and floor
			for (auto &room : rooms)
			{
				room.inflated(wall_size).enumerate_border([&](point2 const& location) {
					result.tiles[location] = build_tile::wall;
				});
				room.enumerate([&](point2 const& location) {
					result.tiles[location] = build_tile::floor;
				});
			}

			build_house_entrance(rooms, house_bounds, passages, result, rng); // select room for entrance and place outside door
			build_house_doors(rooms, passages, result, rng); // interconnect rooms and place doors
			build_house_placeables(rooms, passages, placeables_density, result, rng); // add placeables and habitants
		}

		// select excluded rooms
		template<typename Generator>
		void build_house_rooms(rectangle const& house_bounds, unsigned int room_size, std::vector<rectangle> &rooms, Generator &rng)
		{
			bsp<Generator> room_bsp(rng, house_bounds, room_size, wall_size); // room_zise rooms with 1-tile wall

			rooms.reserve(room_bsp.count());
			room_bsp.enumerate_bounds([&](auto const& room) {
				rooms.push_back(room);
			});

			unsigned int count = room_bsp.count();

			if (count >= 9) // 3x3 - can cut corners
			{
				room_bsp.enumerate_bounds([&](auto const& room) {
					if (room.inflated(wall_size).touch_corner(house_bounds) && std::uniform_int_distribution<int>{0, 1}(rng) == 0)
					{
						rooms.erase(std::remove(rooms.begin(), rooms.end(), room));
					}
				});
			}
			else if (count >= 4) // 2x2 - at least can cut one corner
			{
				if (std::uniform_int_distribution<int>{0, 1}(rng) == 0)
				{
					std::vector<rectangle> cut_candidates;
					cut_candidates.reserve(3); // not >= 4

					room_bsp.enumerate_bounds([&](auto const& room) {
						if (room.inflated(wall_size).touch_corner(house_bounds))
						{
							cut_candidates.push_back(room);
						}
					});

					rooms.erase(std::remove(rooms.begin(), rooms.end(), random_item(cut_candidates, rng)));
				}
			}
		}

		// select entry room in house
		template<typename Generator>
		rectangle build_house_entrance(std::vector<rectangle> const& rooms, rectangle const& bounds, std::vector<rectangle> & passages, build_result &result, Generator &rng)
		{
			// detect entrance door candidates
			std::vector<rectangle> entrance_room_candidates;
			entrance_room_candidates.reserve(rooms.size()); // most rooms at border anyway
			for (auto &room : rooms)
			{
				rectangle room_inflated = room.inflated(wall_size + 1);
				if ((room_inflated & bounds) != room_inflated) // touching outside wall
				{
					entrance_room_candidates.push_back(room);
				}
			}

			// select entrance door
			rectangle entrance_room = random_item(entrance_room_candidates, rng);
			std::vector<point2> entrance_door_candidates;
			entrance_door_candidates.reserve(entrance_room.perimeter());
			auto room_inflated = entrance_room.inflated(wall_size);
			room_inflated.enumerate_border([&](auto const& location)
			{
				if (bounds.is_border(location) && !room_inflated.is_corner(location)) entrance_door_candidates.push_back(location);
			});
			auto door = random_item(entrance_door_candidates, rng);

			passages.push_back(rectangle(door, point2(wall_size, wall_size)).inflated(passage_size));

			result.tiles[door] = build_tile::door_ark;

			return entrance_room;
		}

		// interconnect rooms in house
		// - rooms - array of room rectangles
		// - passage - space near doors required for movement throught merged into this array
		template<typename Generator>
		void build_house_doors(std::vector<rectangle> const& rooms, std::vector<rectangle> &passage, build_result &result, Generator &rng)
		{
			build_house_doors(rooms, random_item(rooms, rng), passage, result, rng);
		}

		// interconnect rooms in house starting from specified room
		// - rooms - array of room rectangles
		// - entrance_room - room to start from
		// - passage - space near doors required for movement throught merged into this array
		template<typename Generator>
		void build_house_doors(std::vector<rectangle> const& rooms, rectangle const& entrance_room, std::vector<rectangle> &passages, build_result &result, Generator &rng)
		{
			std::vector<rectangle> untangled;
			std::vector<rectangle> tangled;
			untangled.reserve(rooms.size());
			tangled.reserve(rooms.size());

			for (auto &room : rooms)
			{
				(room == entrance_room ? tangled : untangled).push_back(room);
			}

			while (!untangled.empty())
			{
				std::vector<std::tuple<rectangle, rectangle>> tangle_candidates; // (untangled, tangled) pair
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

				auto tangle = random_item(tangle_candidates, rng); // (untangled, tangled) tuple
				auto doorway = shrink_ark(std::get<0>(tangle).inflated(wall_size) & std::get<1>(tangle).inflated(wall_size), wall_size);
				bool last = untangled.size() == 1;

				point2 door;
				switch (last ? 0 : std::uniform_int_distribution<int>(0, 3)(rng)) // 0, 1, 2 - door (opened, closed, locked), 3, 4, 5 - gap (1 tile, random size, full)
				{
				case 0:
				case 1:
				case 2:
					door = random_point(doorway, rng);
					passages.push_back(inflate_passage(rectangle(door, point2(wall_size, wall_size)), doorway, passage_size));
					result.tiles[door] = build_tile::door_ark;
					break;
				case 3:
					door = random_point(doorway, rng);
					passages.push_back(inflate_passage(rectangle(door, point2(wall_size, wall_size)), doorway, passage_size));
					result.tiles[door] = build_tile::floor;
					break;
				}

				untangled.erase(std::remove(std::begin(untangled), std::end(untangled), std::get<0>(tangle)), std::end(untangled));
				tangled.push_back(std::get<0>(tangle));
			}
		}

		// add habitants and furniture
		// - rooms - array of rooms
		// - passage - space to be empty (i.e reserve place for movement or/and extra object)
		// - density [0, 1] - chance of spawn
		template<typename Real, typename Generator>
		void build_house_placeables(std::vector<rectangle> const& rooms, std::vector<rectangle> const& passage, Real density, build_result &result, Generator &rng)
		{
			for (auto const& room : rooms)
			{
				// habitants

				point2 spawn = random_point(room, rng);
				if (!result.exists(spawn))
				{
					result.placeables.emplace_back(spawn, build_placeable::mobile);
				}

				// furniture

				room.enumerate_border([&](auto const& location)
				{
					if (std::uniform_real_distribution<Real>{}(rng) < density && place_available(passage, result, location))
					{
						result.placeables.emplace_back(location, build_placeable::furniture);
					}
				});
				auto center = room.start() + room.range() / 2;
				if (std::uniform_real_distribution<Real>{}(rng) < density && place_available(passage, result, center))
				{
					result.placeables.emplace_back(center, build_placeable::furniture_table);
				}
			}
		}
	}
}