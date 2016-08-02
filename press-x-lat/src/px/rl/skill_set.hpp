// name: skill_set.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_RL_SKILL_SET_HPP
#define PX_RL_SKILL_SET_HPP

#include <px/rl/skill_book.hpp>

#include <vector>

namespace px
{
	namespace rl
	{
		template <typename Tag, typename User, typename Target>
		class skill_set
		{
		public:
			typedef Tag tag_type;
			typedef skill<User, Target> skill_type;
			typedef skill_book<Tag, User, Target> skillbook_type;

			struct record
			{
			public:
				tag_type tag;
				skill_type ability;
				record(tag_type t, skill_type s) : tag(t), ability(s) {}
			};
		private:
			std::vector<record> m_skills;
			skillbook_type* m_provider;

		public:
			void provide(skillbook_type* book)
			{
				m_provider = book;
			}
			auto add_skill(tag_type tag, size_t slot)
			{
				if (!m_provider) throw std::runtime_error("px::rl::skill_set::add(name, slot) - book provider is null");

				auto it = m_provider->find(tag);

				if (it == m_provider->end()) throw std::runtime_error("px::rl::skill_set::add(name, slot) - book has no skill with name " + tag);

				m_skills.reserve(slot + 1);
				m_skills.emplace(m_skills.begin() + slot, tag, it->second);

				return skill(slot);
			}
			auto add_skill(tag_type tag)
			{
				return add_skill(tag, m_skills.size());
			}
			size_t skils_total() const
			{
				return m_skills.size();
			}

			skill_type* skill(size_t slot)
			{
				return (slot < m_skills.size()) ? &(m_skills[slot].ability) : nullptr;
			}
			template <size_t Slot>
			skill_type* skill()
			{
				return (Slot < m_skills.size()) ? &(m_skills[Slot].ability) : nullptr;
			}
		};
	}
}

#endif