// name: i_game_control.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_CORE_I_GAME_CONTROL_HPP
#define PX_CORE_I_GAME_CONTROL_HPP

#include <px/common/point.hpp>

namespace px
{
	namespace core
	{
		class i_game_control
		{
		public:
			i_game_control() {}
			virtual ~i_game_control() {}

		protected:
			virtual bool player_use(unsigned int ability_slot, point2 target) { return false; }
			virtual bool player_activate(point2 target) { return false; }
			virtual bool player_step(point2 direction) { return false; }

		public:
			bool use(unsigned int ability_slot, point2 target) { return player_use(ability_slot, target); }
			bool activate(point2 target) { return player_activate(target); }
			bool step(point2 direction) { return player_step(direction); }
		};
	}
}

#endif