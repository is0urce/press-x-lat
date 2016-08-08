// name: unit.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_CORE_UNIT_HPP
#define PX_CORE_UNIT_HPP

#include <px/common/toggle.hpp>
#include <px/es/component_collection.hpp>
#include "persistency.hpp"

namespace px
{
	namespace core
	{
		class unit : public es::component_collection
		{
		public:
			persistency get_persistency() const { return m_persistency; }
			void set_persistency(persistency flag) { m_persistency = flag; }
			void destroy() { m_persistency = persistency::destroying; }
			void make_permanent() { m_persistency = persistency::permanent; }
			void make_temporary() { m_persistency = persistency::temporary; }
			void set_location(location_component* location) { m_location = location; }
			location_component* location() { return m_location; }

		public:
			unit() : m_persistency(persistency::serialized) {}

		private:
			persistency m_persistency;
			location_component* m_location;
		};
	}
}

#endif