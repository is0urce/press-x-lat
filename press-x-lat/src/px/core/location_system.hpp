// name: location_system.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_CORE_LOCATION_SYSTEM_HPP
#define PX_CORE_LOCATION_SYSTEM_HPP

#include <px/es/i_system.hpp>
#include <px/es/manager.hpp>

#include "location_component.hpp"

namespace
{
	unsigned int space_width = 64; // should be power of 2, actual size not matters, as it resises
}

namespace px
{
	namespace core
	{
		class location_system : public es::i_system, protected es::manager<location_component, 10000>
		{
		private:
			qtree<location_component*> m_space;

		public:
			location_system() : m_space(space_width) {}
			virtual ~location_system() {}

		protected:
			virtual void element_allocated(location_component &l) override
			{
				l.incarnate(&m_space);
			}
			virtual void element_released(location_component &l) override
			{
				l.incarnate(nullptr);
			}

		public:
			auto make_location(point2 position)-> decltype(make())
			{
				auto component = make();
				component->move(position);
				return component;
			}
		};
	}
}

#endif