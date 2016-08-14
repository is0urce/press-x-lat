// name: character.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_RL_CHARACTER_HPP
#define PX_RL_CHARACTER_HPP

#include <px/rl/entity.hpp>
#include <px/rl/skill_set.hpp>

namespace px
{
	namespace rl
	{
		template <typename SkillTarget>
		class character
			: public skill_set<std::string, SkillTarget, SkillTarget>
			, public entity
		{
		};
	}
}

#endif