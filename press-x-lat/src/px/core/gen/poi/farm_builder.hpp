// name: farm_builder.hpp
// type: c++ header
// desc: struct declaration
// auth: is0urce

#ifndef PX_CORE_GEN_POI_FARM_BUILDER_HPP
#define PX_CORE_GEN_POI_FARM_BUILDER_HPP

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
		}
		class farm_builder : public builder
		{
		public:
			farm_builder() {}
			~farm_builder() {}

		public:
			void build(unsigned int seed, rectangle const& bounds, build_result &result) const override
			{
				std::mt19937 rng(seed);
				std::uniform_int_distribution<unsigned int> sector_chance(0, 1);
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

						switch (sector_chance(rng))
						{
						case 0:
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

								auto house_bounds = rectangle(sector_bounds.start(),{ w, h });
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
						case 1:
							crops = true;
							sector_bounds.enumerate([&](auto const& location) {
								result.tiles[location] = build_tile::soil;
							});
							break;
						default:
							break;
						}
					});
					done = living && crops && true;
				}
			}
		};
	}
}

#endif