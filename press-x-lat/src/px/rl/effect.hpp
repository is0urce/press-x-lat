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

			str_bonus,
			dex_bonus,
			int_bonus,

			// item specific props

			equipment,
			weight,
			value,

			weapon_damage,
			armor_resist,

			ore_power,

			not_valid,
			max_value = not_valid
		};
	}
}

#endif