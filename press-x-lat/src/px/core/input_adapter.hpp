// name: input_adapter.hpp
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_CORE_INPUT_ADAPTER_HPP
#define PX_CORE_INPUT_ADAPTER_HPP

#include <px/shell/control.hpp>
#include <px/shell/key.hpp>
#include "i_game_control.hpp"

namespace px
{
	using shell::key;
	namespace core
	{
		class input_adapter : public shell::control
		{
		private:
			point2 m_hover;
			i_game_control* m_game;

		public:
			input_adapter(i_game_control& game) : m_game(&game) {}
			virtual ~input_adapter() {};

		public:
			virtual bool key_control(key_type code) override
			{
				bool result = false;
				if (m_game)
				{
					switch (code)
					{
					case key::move_none:
						result = m_game->step({ 0, 0 });
						break;
					case key::move_north:
						result = m_game->step({ 0, 1 });
						break;
					case key::move_south:
						result = m_game->step({ 0, -1 });
						break;
					case key::move_east:
						result = m_game->step({ 1, 0 });
						break;
					case key::move_west:
						result = m_game->step({ -1, 0 });
						break;
					case key::move_southwest:
						result = m_game->step({ -1, -1 });
						break;
					case key::move_southeast:
						result = m_game->step({ 1, -1 });
						break;
					case key::move_northwest:
						result = m_game->step({ -1, 1 });
						break;
					case key::move_northeast:
						result = m_game->step({ 1, 1 });
						break;
					case key::action_use:
						result = m_game->activate(m_hover);
						break;
					case key::action_alt:
						result = m_game->activate(m_hover);
						break;
					case key::action0:
						result = m_game->use(0, m_hover);
						break;
					case key::action1:
						result = m_game->use(1, m_hover);
						break;
					case key::action2:
						result = m_game->use(2, m_hover);
						break;
					case key::action3:
						result = m_game->use(3, m_hover);
						break;
					case key::action4:
						result = m_game->use(4, m_hover);
						break;
					case key::action5:
						result = m_game->use(5, m_hover);
						break;
					default:
						result = false;
						break;
					}
				}
				return result;
			}
			virtual bool hover_control(const point2 &position) override
			{
				m_hover = position;
				return false;
			}
			virtual bool click_control(const point2 &position, unsigned int button) override
			{
				m_hover = position;
				return m_game->activate(m_hover);
			}
			virtual bool scroll_control(int delta) override
			{
				return false;
			}
		};
	}
}

#endif