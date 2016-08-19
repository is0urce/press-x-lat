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

namespace px
{
	namespace core
	{
		class farm_builder : public builder
		{
		public:
			farm_builder() {}
			~farm_builder() {}

		public:
			void build(unsigned int seed, rectangle const& bounds, build_result &result) const override
			{
				bounds.enumerate([&](const point2& location) {
					result.tiles[location] = build_tile::no_change;
				});

				//std::uniform_int_distribution<unsigned int> furniture_chance(1, 100);
				//std::uniform_int_distribution<unsigned int> monster_chance(1, 100);

				std::uniform_int_distribution<unsigned int> sector_chance(0, 3);

				std::mt19937 rng(seed);

				fn::bsp<> sector_bsp(rng, bounds, 15);
				sector_bsp.enumerate([&](auto const& sector) {

					switch (sector_chance(rng))
					{
					case 0:
						{
							int w = sector.bounds.width();
							int h = sector.bounds.height();
							std::uniform_int_distribution<int> wd(w / 4 * 3, w);
							std::uniform_int_distribution<int> hd(h / 4 * 3, h);
							w = wd(rng) - 2;
							h = hd(rng) - 2;
							w = w < 0 ? 0 : w;
							h = h < 0 ? 0 : h;
							fn::bsp<> room_bsp(rng, { sector.bounds.start(), { w, h } }, 5);
							room_bsp.enumerate([&](auto const& room) {

								room.bounds.enumerate([&](point2 const& location) {
									result.tiles[location] = build_tile::floor;
								});

								room.bounds.enumerate_border([&](point2 const& location) {
									result.tiles[location] = build_tile::wall_inside;
								});

							});
						}
						break;
					case 1:
						sector.bounds.enumerate([&](auto const& location) {
							result.tiles[location] = build_tile::soil;
						});
						break;
					default:
						break;
					}

					//auto furniture_line = room.bounds.deflated(1);

					//furniture_line.enumerate_bounds([&](const point2& location) {
					//	if (furniture_chance(rng) <= 20)
					//	{
					//		result.placeables.emplace_back(location, build_placeable::furniture);
					//	}
					//	if (monster_chance(rng) <= 70)
					//	{
					//		result.placeables.emplace_back(room.bounds.start() + room.bounds.range() / 2, build_placeable::mobile);
					//		if (monster_chance(rng) <= 30)
					//		{
					//			result.placeables.emplace_back(room.bounds.start() + room.bounds.range() / 2, build_placeable::mobile);
					//			if (monster_chance(rng) <= 10)
					//			{
					//				result.placeables.emplace_back(room.bounds.start() + room.bounds.range() / 2, build_placeable::mobile);
					//			}
					//		}
					//	}
					//});

					//if (furniture_chance(rng) <= 20)
					//{
					//	result.placeables.emplace_back(room.bounds.start() + room.bounds.range() / 2, build_placeable::table);
					//}
				});
			}
		};

	}
}

#endif