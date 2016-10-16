// name: dialogue.hpp
// type: c++ header
// desc: template class
// auth: is0urce

#ifndef PX_RL_DIALOGUE_HPP
#define PX_RL_DIALOGUE_HPP

#include <px/rl/dialogue_reply.hpp>
#include <px/rl/dialogue_state.hpp>

#include <algorithm>

namespace px
{
	namespace rl
	{
		template <typename Node>
		class dialogue
		{
		public:
			typedef dialogue_reply<Node> reply_type;

		public:
			void add_reply(Node&& node)
			{
				m_lines.emplace_back(node);
			}
			void link_answers(size_t n, std::vector<size_t> answers)
			{
				std::vector<reply_type const*> links(answers.size());
				std::transform(std::begin(answers), std::end(answers), std::begin(links), [this](auto const& answer) { return &m_lines[answer]; });

				m_lines[n].link(links);
			}
			template <typename StateData = void*>
			dialogue_state<StateData, Node> start(size_t root, StateData data = StateData())
			{
				dialogue_state<StateData, Node> state(data, &m_lines[root]);
				return state;
			}

		public:
			dialogue() {}
			dialogue(size_t reserve) : dialogue_reply()
			{
				m_lines.reserve(reserve);
			}

		private:
			std::vector<reply_type> m_lines;
		};
	}
}

#endif