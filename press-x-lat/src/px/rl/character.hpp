// name: character.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_RL_CHARACTER_HPP
#define PX_RL_CHARACTER_HPP

#include <px/rl/ability.hpp>

#include <vector>

namespace px
{
	namespace rl
	{
		template <typename _T>
		class character
		{
		public:
			typedef _T target;
			typedef delegate_ability<character, target> skill;
		public:
			std::vector<skill> m_skills;

		public:
			void add_skill(skill s)
			{
				m_skills.push_back(s);
			}
			skill* get_skill(size_t n)
			{
				return (m_skills.size() > n) ? &m_skills[n] : nullptr;
			}
		};
	}
}

#endif