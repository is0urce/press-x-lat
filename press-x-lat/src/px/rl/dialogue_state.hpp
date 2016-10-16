// name: dialogue_state.hpp
// type: c++ header
// desc: template class
// auth: is0urce

#ifndef PX_RL_DIALOGUE_STATE_HPP
#define PX_RL_DIALOGUE_STATE_HPP

#include <px/rl/dialogue_evaluator.hpp>

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
			StateData const* data() const noexcept
			{
				return m_data;
			}
			StateData* data() noexcept
			{
				return &m_data;
			}

			// uses reply by visible #, skipping invisible variants
			void select(size_t variant)
			{
				size_t index = 0;
				size_t count = 0;
				size_t size = m_visible.size();
				bool visible = m_visible[index];
				while ((!visible || count != variant) && index < size)
				{
					if (visible) ++count;
					++index;
					visible = m_visible[index];
				}
				select_absolute(index);
			}
			// uses first visible reply option (same as select(0))
			void select_first()
			{
				for (size_t index = 0; index < m_current->size(); ++index)
				{
					if (m_visible[index])
					{
						select_absolute(index);
						return;
					}
				}
			}
			bool is_final() const noexcept
			{
				return m_current->is_final();
			}
			bool is_visible(size_t index) const noexcept
			{
				return index < m_visible.size() ? m_visible[index] : false;
			}
			size_t total() const noexcept
			{
				return m_visible->size();
			}
			size_t visible() const noexcept
			{
				size_t n = 0;
				for (auto v : m_visible)
				{
					if (v) ++n;
				}
				return n;
			}
			void recalculate_visible()
			{
				m_visible.resize(m_current->size(), false);
				for (size_t i = 0, size = m_visible.size(); i != size; ++i)
				{
					m_visible[i] = m_script.conditional(*(m_current->select(i)), m_data);
				}
			}

		public:
			dialogue_state(dialogue_reply<Node> const* start, StateData data, dialogue_evaluator<Node, StateData> evaluator)
				: m_data(data)
				, m_current(start)
				, m_script(evaluator)
			{
				recalculate_visible();
			}

		private:
			void select_absolute(size_t index)
			{
				if (m_current->contains(index))
				{
					m_current = m_current->select(index);
					m_script.action(*m_current, m_data);
					recalculate_visible();
				}
			}

		private:
			dialogue_reply<Node> const* m_current;
			std::vector<bool> m_visible;

			StateData m_data;
			dialogue_evaluator<Node, StateData> m_script;
		};
	}
}

#endif