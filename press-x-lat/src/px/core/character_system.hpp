// name: character_system.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_CORE_CHARACTER_SYSTEM_HPP
#define PX_CORE_CHARACTER_SYSTEM_HPP

#include <px/es/i_system.hpp>
#include <px/es/manager.hpp>

#include "character_component.hpp"

#include <map>

namespace px
{
	namespace core
	{
		class character_system
			: public es::i_system
			, public es::manager<character_component, 10000>
		{
		private:
			std::map<std::string, bool> m_spellbook;

		public:
			character_system() {}
			virtual ~character_system() {}
		};
	}
}

#endif