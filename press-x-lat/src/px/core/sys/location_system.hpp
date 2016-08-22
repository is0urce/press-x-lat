// name: location_system.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_CORE_LOCATION_SYSTEM_HPP
#define PX_CORE_LOCATION_SYSTEM_HPP

#include <px/es/i_system.hpp>
#include <px/es/manager.hpp>

#include "location_component.hpp"

namespace px
{
	namespace core
	{
		class location_system
			: public es::i_system
			, public es::manager<location_component, 10000>
		{
		public:
			typedef qtree<location_component*> space_type;

		public:
			std::shared_ptr<location_component> make_location(point2 position)
			{
				auto component = make();
				component->move(position);
				return component;
			}

		public:
			location_system(space_type &space) : m_space(&space) {}
			virtual ~location_system() {}

		protected:
			virtual void element_allocated(location_component &element) override
			{
				element.deactivate();
				element.incarnate(m_space);
			}
			virtual void element_released(location_component &element) override
			{
				element.deactivate();
				element.incarnate(nullptr);
			}

		private:
			space_type* m_space;
		};
	}
}

#endif