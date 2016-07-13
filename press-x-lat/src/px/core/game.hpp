// name: game.hpp
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_CORE_GAME_HPP
#define PX_CORE_GAME_HPP

#include "environment.hpp"
#include "i_game_control.hpp"

namespace px
{
	namespace core
	{
		class game : public environment, public i_game_control
		{
		public:
			game() {}
			virtual ~game() {}

		protected:
			virtual bool player_use(unsigned int ability_slot, point2 target) override
			{
				return false;
			}
			virtual bool player_activate(point2 target) override
			{
				return false;
			}
			virtual bool player_step(point2 direction) override
			{
				auto pawn = player();
				maneuver(*pawn, pawn->current() + direction);
				return false;
			}
		};
	}
}

#endif