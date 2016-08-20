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
			unsigned int sector_size = 15;
			unsigned int room_size = 5;

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
			std::mt19937 rng(seed);
			std::uniform_int_distribution<unsigned int> sector_distribution(static_cast<unsigned int>(farm_sector::min_value), static_cast<unsigned int>(farm_sector::max_value));
			//std::uniform_int_distribution<unsigned int> furniture_chance(1, 100);
			//std::uniform_int_distribution<unsigned int> monster_chance(1, 100);

			bool done = false;
			while (!done)
			{
				bool living = false;
				bool crops = false;

				bounds.enumerate([&](const point2& location) {
					result.tiles[location] = build_tile::no_change;
				});

				fn::bsp<decltype(rng)>::enumerate(rng, bounds, sector_size, [&](auto const& sector) {

					auto sector_bounds = sector.bounds.deflated(1); // some space between "districts"

					switch (static_cast<farm_sector>(sector_distribution(rng)))
					{
					case farm_sector::house:
						living = true;
						{
							int w = sector_bounds.width();
							int h = sector_bounds.height();
							std::uniform_int_distribution<int> wd(w / 4 * 3, w);
							std::uniform_int_distribution<int> hd(h / 4 * 3, h);
							w = wd(rng) - 2;
							h = hd(rng) - 2;
							w = w < 0 ? 0 : w;
							h = h < 0 ? 0 : h;

							auto house_bounds = rectangle(sector_bounds.start(), { w, h });
							fn::bsp<decltype(rng), bool> room_bsp(rng, house_bounds, room_size);
							room_bsp.enumerate([&](auto const& room) {

								room.bounds.enumerate([&](point2 const& location) {
									result.tiles[location] = build_tile::floor;
								});

								room.bounds.enumerate_border([&](point2 const& location) {
									result.tiles[location] = build_tile::wall_inside;
								});

								rectangle room_inflated = room.bounds.inflated(1);

								if (room_inflated.intersection(house_bounds) != room_inflated) // near wall
								{
									result.tiles[room.bounds.start().moved_axis<0>(room.bounds.width() / 2)] = build_tile::floor;
								}
							});
						}
						break;
					case farm_sector::barn:
					case farm_sector::field:
						crops = true;
						sector_bounds.enumerate([&](auto const& location) {
							result.tiles[location] = build_tile::soil;
						});
						break;
					case farm_sector::garden:
					default:
						break;
					}
				});
				done = living && crops && true;
			}
		}
	}
}