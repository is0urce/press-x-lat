// name: npc_component.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_CORE_NPC_COMPONENT_HPP
#define PX_CORE_NPC_COMPONENT_HPP

#include <px/es/i_component.hpp>
#include <px/es/component_link.hpp>

namespace px
{
	namespace core
	{
		class environment;
		class location_component;
		class character_component;
		class npc_component
			: public es::i_component
			, public es::component_link<location_component>
		{
		private:
			bool m_alert;

		public:
			npc_component();
			virtual ~npc_component();

		public:
			void resolve_action(environment &e);
		};
	}
}

#endif