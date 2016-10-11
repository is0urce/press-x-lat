// name: equipment_slot.hpp
// type: c++ header
// desc: enum
// auth: is0urce

#ifndef PX_RL_EQUOPMENT_SLOT_HPP
#define PX_RL_EQUOPMENT_SLOT_HPP

namespace px
{
	namespace rl
	{
		enum class equipment_slot : unsigned int
		{
			hand_main,
			hand_off,
			armor_helmet,
			armor_chest,

			min_value = hand_main,
			max_value = armor_chest
		};
	}
}

#endif