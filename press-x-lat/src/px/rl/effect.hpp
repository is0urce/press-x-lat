// name: effect.hpp
// type: c++ header
// desc: enum
// auth: is0urce

#ifndef PX_RL_EFFECT_HPP
#define PX_RL_EFFECT_HPP

namespace px
{
	namespace rl
	{
		enum class effect : unsigned int
		{
			weapon_damage,

			not_valid,

			max_value = not_valid
		};
	}
}

#endif