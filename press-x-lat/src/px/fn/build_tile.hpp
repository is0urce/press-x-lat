// name: build_tile.hpp
// type: c++ header
// desc: enum
// auth: is0urce

#ifndef PX_FN_BUILD_TILE_HPP
#define PX_FN_BUILD_TILE_HPP

#include <px/common/point.hpp>
#include <px/common/matrix.hpp>

#include <list>

namespace px
{
	namespace fn
	{
		enum class build_tile : unsigned int
		{
			no_change,

			background,
			background_pass,
			background_block,
			background_flora,
			background_water,

			doodad_log,
			doodad_rock,
			doodad_tombstone,

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

			door_ark
		};
	}
}

#endif