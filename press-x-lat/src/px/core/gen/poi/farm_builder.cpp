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

			enum class farm_sector : unsigned int
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
			std::uniform_int_distribution<unsigned int> sector_distribution(static_cast<unsigned int>(farm_sector::min_value), static_cast<unsigned int>(farm_sector::max_value));
			//std::uniform_int_distribution<unsigned int> furniture_chance(1, 100);
			//std::uniform_int_distribution<unsigned int> monster_chance(1, 100);

			bool done = false;
			while (!done)
			{
				bool living = false;
				bool food = false;

				bounds.enumerate([&](const point2& location) {
					result.tiles[location] = build_tile::no_change;
				});

				fn::bsp<rng_type>::enumerate(rng, bounds, sector_size, [&](auto const& sector) {

					auto sector_bounds = sector.bounds.deflated(1); // some space between "districts"

					switch (static_cast<farm_sector>(sector_distribution(rng)))
					{
					case farm_sector::house:
						living = true;
						{
							int w = sector_bounds.width();
							int h = sector_bounds.height();
							int shrink_x = std::uniform_int_distribution<int>(0, w / 4)(rng);
							int shrink_y = std::uniform_int_distribution<int>(0, w / 4)(rng);
							int move_x = std::uniform_int_distribution<int>(0, shrink_x)(rng);
							int move_y = std::uniform_int_distribution<int>(0, shrink_y)(rng);

							auto house_bounds = rectangle(sector_bounds.start().moved(move_x, move_y), { w - shrink_x, h - shrink_y });
							house_bounds.enumerate([&](auto const& location) {
								result.tiles[location] = build_tile::wall_inside;
							});

							fn::bsp<rng_type> room_bsp(rng, house_bounds, room_size, wall_size); // room_zise rooms with 1-tile wall

							std::vector<fn::bsp<rng_type>::node*> entrance_room_candidates;
							entrance_room_candidates.reserve(room_bsp.count()); // most of rooms at border anyway

							room_bsp.enumerate([&](auto & room) {

								// add entrance room candidate
								rectangle room_inflated = room.bounds.inflated(wall_size + 1);
								if ((room_inflated & house_bounds) != room_inflated) // touching outside wall
								{
									entrance_room_candidates.push_back(&room);
								}

								// draw
								room.bounds.enumerate([&](point2 const& location) {
									result.tiles[location] = build_tile::floor;
								});
							});

							if (entrance_room_candidates.size() > 0)
							{
								auto &entrance_room_bounds = random_item(entrance_room_candidates, rng)->bounds;
								std::vector<point2> entrance_door_candidates;
								entrance_door_candidates.reserve(entrance_room_bounds.range().x() + entrance_room_bounds.range().y() * 2); // perimeter

								auto room_inflated = entrance_room_bounds.inflated(wall_size);
								room_inflated.enumerate_border([&](auto const& location)
								{
									if (house_bounds.is_border(location) && !room_inflated.is_corner(location)) entrance_door_candidates.push_back(location);
								});

								if (entrance_door_candidates.size() > 0)
								{
									point2 entrance_door = random_item(entrance_door_candidates, rng);
									result.tiles[entrance_door] = build_tile::floor;
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