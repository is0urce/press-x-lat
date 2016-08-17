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

#include <px/rl/tile.hpp>

#include <list>

namespace px
{
	namespace core
	{
		enum build_tile : unsigned int
		{
			no_change,
			background,
			background_pass,
			background_block,
			background_flora,
			background_doodad,
			background_water,

			grass,
			snow,
			sand,
			gravel,
			stone,
			ice,
			water,
			lava,
			chasm,

			floor,
			wall,
			wall_outside,
			wall_inside,

			door_ark,

			max_value = door_ark
		};
		enum build_placeable : unsigned int
		{
			animal,
			animal_hunter,
			animal_prey,

			mobile,
			mobile_champion,
			mobile_boss,
			mobile_leader,

			door,
			door_heavy,
			door_locked,

			furniture,
			table,
			chair,
			bed,
			shelf,

			chest,
			chest_loot,
			chest_score,

			flora_bloom,

			torch,
			braiser,

			statue,

			max_vaule = statue
		};

		struct placeable_entry
		{
			point2 location;
			build_placeable placeable;
			placeable_entry(point2 l, build_placeable p) : location(l), placeable(p) {}
		};

		struct build_result
		{
			px::matrix2<build_tile> tiles;
			std::list<placeable_entry> placeables;
		};

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

		class mapper
		{
		public:
			typedef std::shared_ptr<unit> unit_ptr;
			typedef rl::tile<image> tile_type;
			typedef matrix2<tile_type, settings::cell_width, settings::cell_height> map_type;

		public:
			void map(build_result build, const point2 &world_location, map_type& terrain, bool static_mobiles, std::list<unit_ptr>& units)
			{
			}
		};
	}
}

#endif