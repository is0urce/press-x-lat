// name: npc_component.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_CORE_NPC_COMPONENT_HPP
#define PX_CORE_NPC_COMPONENT_HPP

#include <px/es/i_component.hpp>
#include <px/es/component_link.hpp>
#include <px/core/sys/i_useable_component.hpp>

namespace px
{
	namespace core
	{
		class environment;
		class location_component;

		class npc_component
			: public es::i_component
			, public es::component_link<location_component>
			, public i_useable_component
		{
		public:
			void resolve_action(environment &e);

		protected:
			virtual void use_component(location_component* l, environment& env);
			virtual bool useable_component(location_component* l, environment& env) const;

		public:
			npc_component();
			virtual ~npc_component();
		};
	}
}

#endif