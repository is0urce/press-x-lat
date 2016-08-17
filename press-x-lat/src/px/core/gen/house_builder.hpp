// name: house_builder.hpp
// type: c++ header
// desc: struct declaration
// auth: is0urce

#ifndef PX_CORE_HOUSE_BUILDER_HPP
#define PX_CORE_HOUSE_BUILDER_HPP

#include <px/common/rectangle.hpp>
#include <px/common/matrix.hpp>

#include <px/fn/bsp.hpp>

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

			monster,
			monster_champion,
			monster_boss,
			monster_leader,

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
		struct build
		{
			point2 location;
			build_placeable placeable;
		};
		struct build_result
		{
			px::matrix2<build_tile> tiles;
			std::list<build> placeables;
		};
		class house_builder
		{
		public:
			template<typename Generator = std::mt19937>
			build_result build(Generator &rng, const point2& bounds) const
			{
				build_result result;

				//result.tiles.resize(bounds);
				//result.tiles.fill(build_tile::no_change);

				//fn::bsp<Generator> bsp(rng, bounds, 6);
				//std::uniform_int_distribution<unsigned int> furniture_chance(0, 5);
				//bsp.enumerate([&](const auto& room) {

				//	auto furniture_line = room.bounds.deflated(1);

				//	room.bounds.enumerate([](auto x, auto y) {
				//		result.tiles[point2(x, y)] = tile::floor;
				//	});

				//	furniture_line.enumerate_bounds([&](auto x, auto y) {
				//		if (furniture_chance(rng) == 0)
				//		{
				//			result.tiles[point2(x, y)] = build_placeable::furniture;
				//		}
				//	});

				//	if (furniture_chance(rng) == 0)
				//	{
				//		result.tiles[room.bounds.start() + room.bounds.range() / 2] = build_placeable::table;
				//	}
				//});

				return result;
			}
		};
	}
}

#endif