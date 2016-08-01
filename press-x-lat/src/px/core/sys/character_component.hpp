// name: character_component.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_CORE_CHARACTER_COMPONENT_HPP
#define PX_CORE_CHARACTER_COMPONENT_HPP

#include <px/es/i_component.hpp>
#include <px/es/component_link.hpp>

#include <px/rl/character.hpp>


namespace px
{
	namespace core
	{
		class body_component;
		class character_component
			: public es::i_component
			, public rl::character<body_component*>
		{
		public:
			virtual ~character_component() {}
		};
	}
}

#endif