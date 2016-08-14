// name: behavior_system.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_CORE_BEHAVIOR_SYSTEM_HPP
#define PX_CORE_BEHAVIOR_SYSTEM_HPP

#include <px/es/i_system.hpp>
#include <px/es/manager.hpp>

#include "npc_component.hpp"

namespace px
{
	namespace core
	{
		class environment;
		class behavior_system
			: public es::i_system
			, public es::manager<npc_component, 1000>
		{
		private:
			environment* m_environment;

		public:
			behavior_system(environment& e) : m_environment(&e) {}
			virtual ~behavior_system() {}

		protected:
			virtual void element_allocated(npc_component &component) override
			{
				component.deactivate();
			}
			virtual void fixed_update_system() override
			{
				enumerate([this](npc_component &npc)
				{
					if (npc.active())
					{
						npc.resolve_action(*m_environment);
					}
				});
			}
		};
	}
}

#endif