// name: house_builder.hpp
// type: c++ header
// desc: struct declaration
// auth: is0urce

#ifndef PX_CORE_HOUSE_BUILDER_HPP
#define PX_CORE_HOUSE_BUILDER_HPP

#include <px/common/rectangle.hpp>
#include <px/common/matrix.hpp>

#include <px/fn/bsp.hpp>

#include <px/core/unit.hpp>
#include <px/core/image.hpp>
#include <px/core/settings.hpp>
#include <px/core/unit_record.hpp>

#include <px/rl/tile.hpp>

#include <px/core/gen/build_result.hpp>

namespace px
{
	namespace core
	{
		class house_builder
		{
		public:
			template<typename Generator = std::mt19937>
			build_result build(Generator &rng, const point2& bounds) const
			{
				build_result result;

				result.tiles.resize(bounds);
				result.tiles.fill(build_tile::no_change);

				std::uniform_int_distribution<unsigned int> furniture_chance(1, 100);
				std::uniform_int_distribution<unsigned int> monster_chance(1, 100);

				fn::bsp<Generator> bsp(rng, bounds, 6);
				bsp.enumerate([&](const auto& room) {
					room.bounds.enumerate([&](const point2& location) {
						result.tiles[location] = build_tile::floor;
					});
					////room.bounds.enumerate_border([&](const point2& location) {
					////	result.tiles[location] = build_tile::wall_inside;
					////});

					auto furniture_line = room.bounds.deflated(1);

					furniture_line.enumerate_bounds([&](const point2& location) {
						if (furniture_chance(rng) <= 20)
						{
							result.placeables.emplace_back(location, build_placeable::furniture);
						}
						if (monster_chance(rng) <= 70)
						{
							result.placeables.emplace_back(room.bounds.start() + room.bounds.range() / 2, build_placeable::mobile);
							if (monster_chance(rng) <= 30)
							{
								result.placeables.emplace_back(room.bounds.start() + room.bounds.range() / 2, build_placeable::mobile);
								if (monster_chance(rng) <= 10)
								{
									result.placeables.emplace_back(room.bounds.start() + room.bounds.range() / 2, build_placeable::mobile);
								}
							}
						}
					});

					if (furniture_chance(rng) <= 20)
					{
						result.placeables.emplace_back(room.bounds.start() + room.bounds.range() / 2, build_placeable::table);
					}
				});

				return result;
			}
		};

	}
}

#endif