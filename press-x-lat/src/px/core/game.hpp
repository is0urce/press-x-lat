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
			virtual bool player_use(unsigned int slot, point2 target) override
			{
				auto player = m_environment->player();
				bool action = false;
				if (player)
				{
					action = m_environment->cast(*player, slot, target);
				}
				if (action)
				{
					m_environment->turn();
				}
				return action;
			}
			virtual bool player_activate(point2 target) override
			{
				return false;
			}
			virtual bool player_step(point2 direction) override
			{
				auto player = m_environment->player();
				bool action = false;
				if (player)
				{
					if (direction.x() == 0 && direction.y() == 0)
					{
						m_environment->turn();
						action = true;
					}
					else
					{
						action = m_environment->maneuver(*player, player->current() + direction);
					}
				}
				if (action)
				{
					m_environment->turn();
				}
				return action;
			}
			virtual bool player_hover(point2 target_pos) override
			{
				m_environment->target(target_pos);
				return false;
			}
		};
	}
}

#endif