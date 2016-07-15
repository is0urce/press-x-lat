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
		class game : public i_game_control
		{
		private:
			environment* m_environment;
		public:
			game(environment& e) : m_environment(&e)
			{
			}
			virtual ~game()
			{
			}
			game(const game&) = delete;

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
				auto player = m_environment->player();
				bool action = player && m_environment->maneuver(*player, player->current() + direction);
				if (action)
				{
					m_environment->turn();
				}
				return action;
			}
		};
	}
}

#endif