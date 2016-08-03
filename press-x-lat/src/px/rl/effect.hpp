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
			no_effect,

			weapon_damage,
			armor_resist,

			weight_increase,
			value_increase,

			str_bonus,
			dex_bonus,
			int_bonus,

			not_valid,
			max_value = not_valid
		};
	}
}

#endif