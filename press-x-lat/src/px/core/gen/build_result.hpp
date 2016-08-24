// name: terrain_builder.hpp
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_CORE_BUILD_RESULT_HPP
#define PX_CORE_BUILD_RESULT_HPP

#include <px/common/point.hpp>
#include <px/common/matrix.hpp>

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

			soil,
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
			floor_rock,

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

		struct tile_entry
		{
			build_tile tile;
			unsigned int group;
			tile_entry(build_tile t, unsigned int g) : tile(t), group(g) {}
		};

		struct placeable_entry
		{
			point2 location;
			build_placeable placeable;
			unsigned int group;
			placeable_entry(point2 l, build_placeable p) : location(l), placeable(p), group(0) {}
			placeable_entry(point2 l, build_placeable p, unsigned int group_index) : location(l), placeable(p), group(group_index) {}
		};

		struct build_result
		{
			px::matrix2<build_tile> tiles;
			std::list<placeable_entry> placeables;
			bool exists(point2 const& location) const
			{
				for (auto const& placeable : placeables)
				{
					if (placeable.location == location) return true;
				}
				return false;
			}
		};
	}
}

#endif