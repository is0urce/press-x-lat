// name: body_component.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_CORE_BODY_COMPONENT_HPP
#define PX_CORE_BODY_COMPONENT_HPP

#include <px/es/i_component.hpp>
#include <px/es/component_link.hpp>
#include <px/rl/body.hpp>
#include <px/rl/character.hpp>
#include <px/rl/mass.hpp>
#include <px/rl/traverse.hpp>

namespace px
{
	namespace core
	{
		class character_component;
		class body_component
			: public es::i_component
			, public rl::body
			, public rl::mass<static_cast<size_t>(rl::traverse::max_value)>
			, public es::component_link<character_component>
		{
		public:
			virtual ~body_component() {}
		};
	}
}

#endif