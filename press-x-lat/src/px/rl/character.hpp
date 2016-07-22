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
		template <typename _P>
		class skill_set
		{
		public:
			typedef _P provider;
			typedef typename provider::key_type name;
			typedef typename provider::mapped_type skill;
			struct record
			{
				skill ability;
				name tag;
			};
		private:
			provider* m_provider;
			std::vector<record> m_skills;

		public:
			skill_set(provider &skillbook) : m_provider(&skillbook)
			{
			}
		public:
			void add(skill)
			{
				m_skills.emplace_back({ s, {} });
			}
			void add(name tag, size_t slot)
			{
				m_skills.reserve(slot + 1);
				m_skills[slot] = { m_provider[tag], tag };
			}
			skill* get_skill(size_t slot)
			{
				return (m_skills.size() > n) ? &(m_skills[n].ability) : nullptr;
			}
		};

		template <typename _T>
		class character
		{
		public:
			typedef _T target;
			typedef skill<character, target> skill;
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