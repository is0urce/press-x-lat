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
			static const unsigned int room_size = 5;
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
								result.tiles[location] = build_tile::floor;
							});

							fn::bsp<rng_type> room_bsp(rng, house_bounds, room_size);
							std::vector<fn::bsp<rng_type>::node*> enters;
							enters.reserve(room_bsp.count()); // most of rooms at border anyway
							room_bsp.enumerate([&](auto & room) {

								// drawing
								room.bounds.enumerate_border([&](point2 const& location) {
									result.tiles[location] = build_tile::wall_inside;
								});

								rectangle room_inflated = room.bounds.inflated(1);
								if (room_inflated.intersection(house_bounds) != room_inflated) // touching outside wall
								{
									enters.push_back(&room);
								}
							});
							auto &enter_bounds = enters[std::uniform_int_distribution<size_t>(0, enters.size() - 1)(rng)]->bounds;
							result.tiles[enter_bounds.start().moved_axis<0>(enter_bounds.width() / 2)] = build_tile::floor;
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
		}
	}
}