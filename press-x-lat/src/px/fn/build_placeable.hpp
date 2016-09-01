// name: build_placeable.hpp
// type: c++ header
// desc: enum
// auth: is0urce

#ifndef PX_FN_BUILD_PLACEABLE_HPP
#define PX_FN_BUILD_PLACEABLE_HPP

#include <px/common/point.hpp>
#include <px/common/matrix.hpp>

#include <list>

namespace px
{
	namespace fn
	{
		enum class build_placeable : unsigned int
		{
			animal,
			animal_domestic,
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
			furniture_table,
			furniture_chair,
			furniture_bed,
			furniture_shelf,
			furniture_barrel,

			chest,
			chest_loot,
			chest_score,

			flora_bloom,

			light_candle,
			light_torch,
			light_braiser,

			statue,
		};
	}
}

#endif