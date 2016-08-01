// name: character_system.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_CORE_CHARACTER_SYSTEM_HPP
#define PX_CORE_CHARACTER_SYSTEM_HPP

#include <px/es/i_system.hpp>
#include <px/es/manager.hpp>

#include "character_component.hpp"

#include <px/rl/skill.hpp>
#include <px/rl/skill_book.hpp>

namespace px
{
	namespace core
	{
		class body_component;

		class character_system
			: public es::i_system
			, public es::manager<character_component, 10000>
		{
		public:
			typedef rl::skill_book<std::string, body_component*, body_component*> spellbook_type;
			typedef rl::skill<body_component*, body_component*> skill_type;
		private:
			spellbook_type m_spellbook;

		public:
			character_system() {}
			virtual ~character_system() {}

		protected:
			virtual void element_allocated(character_component &l) override
			{
				l.provide(&m_spellbook);
			}

		public:
			spellbook_type& skill_book()
			{
				return m_spellbook;
			}
		};
	}
}

#endif