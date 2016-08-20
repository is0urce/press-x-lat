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
				house,
				field,
				barn,
				garden,

				min_value = house,
				max_value = garden
			};

			rectangle shrink_line(rectangle const& line)
			{
				if (line.range().x() > line.range().y())
				{
					return rectangle(line.start().moved_axis<0>(1), line.range().moved_axis<0>(-1));
				}
				else
				{
					return rectangle(line.start().moved_axis<1>(1), line.range().moved_axis<1>(-1));
				}
			}
			template <typename Generator, typename Item>
			Item const& random_item(std::vector<Item> const& vec, Generator &rng)
			{
				return vec[std::uniform_int_distribution<std::vector<Item>::size_type>(0, vec.size() - 1)(rng)];
			}
			template <typename Generator>
			point2 random_range(point2 const& range, Generator &rng)
			{
				return point2(std::uniform_int_distribution<int>(0, range.x())(rng), std::uniform_int_distribution<int>(0, range.y())(rng));
			}
		}

		farm_builder::farm_builder() {}
		farm_builder::~farm_builder() {}

		void farm_builder::build(unsigned int seed, rectangle const& bounds, build_result &result) const
		{
			rng_type rng(seed);
			//std::uniform_int_distribution<unsigned int> furniture_chance(1, 100);
			//std::uniform_int_distribution<unsigned int> monster_chance(1, 100);

			bool done = false;
			while (!done)
			{
				bool living = false;
				bool food = false;

				bounds.enumerate([&](point2 const& location) {
					result.tiles[location] = build_tile::no_change;
				});

				fn::bsp<rng_type>::enumerate(rng, bounds, sector_size, [&](auto const& sector) {

					auto sector_bounds = sector.bounds.deflated(1); // some space between "districts"

					auto sector_index = std::uniform_int_distribution<int>(static_cast<int>(farm_sector::min_value), static_cast<int>(farm_sector::max_value))(rng);
					switch (static_cast<farm_sector>(sector_index))
					{
					case farm_sector::house:
						living = true;
						{
							point2 shrink = random_range(sector_bounds.range() / 4, rng);
							auto house_bounds = rectangle(sector_bounds.start() + random_range(shrink, rng), sector_bounds.range() - shrink);
							house_bounds.enumerate([&](auto const& location) {
								result.tiles[location] = build_tile::wall;
							});

							fn::bsp<rng_type> room_bsp(rng, house_bounds, room_size, wall_size); // room_zise rooms with 1-tile wall

							std::vector<rectangle> entrance_room_candidates;
							entrance_room_candidates.reserve(room_bsp.count()); // most rooms at border anyway

							room_bsp.enumerate([&](auto & room) {

								// add entrance room candidate
								rectangle room_inflated = room.bounds.inflated(wall_size + 1);
								if ((room_inflated & house_bounds) != room_inflated) // touching outside wall
								{
									entrance_room_candidates.push_back(room.bounds);
								}

								// draw
								room.bounds.enumerate([&](point2 const& location) {
									result.tiles[location] = build_tile::floor;
								});
							});

							if (entrance_room_candidates.size() > 0)
							{
								auto const& entrance_room_bounds = random_item(entrance_room_candidates, rng);
								std::vector<point2> entrance_door_candidates;
								entrance_door_candidates.reserve(entrance_room_bounds.perimeter());

								auto room_inflated = entrance_room_bounds.inflated(wall_size);
								room_inflated.enumerate_border([&](auto const& location)
								{
									if (house_bounds.is_border(location) && !room_inflated.is_corner(location)) entrance_door_candidates.push_back(location);
								});

								if (entrance_door_candidates.size() > 0)
								{
									point2 entrance_door = random_item(entrance_door_candidates, rng);
									result.tiles[entrance_door] = build_tile::door_ark;
								}
							}
						}
						break;
					case farm_sector::barn:
					case farm_sector::field:
						food = true;
						sector_bounds.enumerate([&](auto const& location) {
							result.tiles[location] = build_tile::soil;
						});
						break;
					case farm_sector::garden:
						food = true;
					default:
						break;
					}
				});
				done = living && food;
			}
		} // build() method
	}
}