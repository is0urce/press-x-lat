// name: dialogue_state.hpp
// type: c++ header
// desc: template class
// auth: is0urce

#ifndef PX_RL_DIALOGUE_STATE_HPP
#define PX_RL_DIALOGUE_STATE_HPP

#include <vector>

namespace px
{
	namespace rl
	{
		template <typename Node> class dialogue_reply;

		template <typename StateData, typename Node>
		class dialogue_state
		{
		public:
			dialogue_reply<Node> const* current() const noexcept
			{
				return m_current;
			}
			void select(size_t variant)
			{
				m_current = m_current->select(variant);
				//m_script.action(m_current, m_data);
				// if (m_current.is_final() m_script.finish(m_current, m_data)
			}

			dialogue_state(StateData data, dialogue_reply<Node> const* start) : m_data(data), m_current(start)
			{
				calculate_visible();
			}
		private:
			void calculate_visible()
			{
				m_visible.assign(m_current->size(), false);
				for (size_t i = 0, size = m_current->size(); i != size; ++i)
				{
					m_visible[i] = true; // m_script.conditional(next, m_data);
				}
			}

		private:
			dialogue_reply<Node> const* m_current;
			std::vector<bool> m_visible;

			StateData m_data;
			//Evaluator m_script;
		};
	}
}

#endif