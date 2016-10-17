// name: body_component.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_CORE_BODY_COMPONENT_HPP
#define PX_CORE_BODY_COMPONENT_HPP

#include <px/es/i_component.hpp>
#include <px/es/component_link.hpp>

// meat
#include <px/rl/actor.hpp>
#include <px/rl/mass.hpp>

// enums
#include <px/rl/effect.hpp>
#include <px/rl/equipment_slot.hpp>
#include <px/rl/traverse.hpp>

#include <functional>
#include <memory>
#include <list>
#include <vector>

namespace px
{
	namespace core
	{
		class i_useable_component;
		class character_component;
		class environment;

		class body_component
			: public es::i_component
			, public es::component_link<character_component>
			, public es::component_link<i_useable_component>
			, public rl::actor<body_component, rl::effect, rl::equipment_slot, environment>
			, public rl::mass<static_cast<size_t>(rl::traverse::max_value) + 1>
		{
		public:
			using es::component_link<character_component>::link;
			using es::component_link<i_useable_component>::link;

		public:
			body_component() {}
			virtual ~body_component() {}
		};
	}
}

#endif