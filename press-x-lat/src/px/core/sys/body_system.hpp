// name: body_system.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_CORE_BODY_SYSTEM_HPP
#define PX_CORE_BODY_SYSTEM_HPP

#include <px/es/i_system.hpp>
#include <px/es/manager.hpp>

#include "body_component.hpp"

namespace px
{
	namespace core
	{
		class body_system
			: public es::i_system
			, public es::manager<body_component, 100>
		{
		public:
			body_system() {}
			virtual ~body_system() {}

		protected:
			virtual void element_allocated(body_component &component) override
			{
				reset_component(component);
			}
			virtual void element_released(body_component &component) override
			{
				reset_component(component);
			}
			virtual void update_system() override
			{
				enumerate([](body_component &body)
				{
					if (body.active())
					{
						if (body.dead())
						{
							body.make_traversable(rl::traverse::floor);
						}
					}
				});
			}
		private:
			void reset_component(body_component &component)
			{
				component.deactivate();
				component.clear_faction();
				component.clear_equipment();
			}
		};
	}
}

#endif