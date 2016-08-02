// name: skill_book.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_RL_SKILL_BOOK_HPP
#define PX_RL_SKILL_BOOK_HPP

#include <px/rl/skill.hpp>

#include <map>

namespace px
{
	namespace rl
	{
		template <typename Tag, typename User, typename Target>
		class skill_book : public std::map<Tag, rl::skill<User, Target>>
		{
		public:
			typedef rl::skill<User, Target> skill_type;
			typedef typename skill_type::target_fn tf;
			typedef typename skill_type::target_check_fn tfc;
			typedef typename skill_type::ground_fn gf;
			typedef typename skill_type::ground_check_fn gfc;

		public:
			skill_type& add_target(Tag name, tf f, tfc c)
			{
				auto pair = emplace(name, skill_type{ f, c });

				if (!pair.second) throw std::runtime_error("px::rl::skill_book::add_target(..) - can't insert new skill (same tag exists)");

				return pair.first->second; // value of an iterator
			}
			skill_type& add_ground(Tag name, gf f, gfc c)
			{
				auto pair = emplace(name, skill_type{ gf, gfc }).first;

				if (!pair.second) throw std::runtime_error("px::rl::skill_book::add_target(..) - can't insert new skill (same tag exists)");

				return pair.first->second; // value of an iterator
			}
		};
	}
}

#endif