// name: character_system.cpp
// type: c++ header
// desc: class methods implementation
// auth: is0urce

#include "character_system.hpp"

namespace px
{
	namespace core
	{
		character_system::character_system() {}
		character_system::~character_system() {}

		void character_system::element_allocated(character_component &l)
		{
			l.provide(&m_spellbook);
		}
	}
}
