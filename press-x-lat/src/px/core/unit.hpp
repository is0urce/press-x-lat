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
			void set_location(location_component* location) { m_location = location; }
			location_component* location() { return m_location; }
			persistency get_persistency() const { return m_persistency; }
			void set_persistency(persistency flag) { m_persistency = flag; }
			void make_permanent() { m_persistency = persistency::permanent; }
			void make_temporary() { m_persistency = persistency::temporary; }
			void destroy() { m_persistency = persistency::destroying; m_decay = 0; }
			void destroy(unsigned int decay_timer) { m_persistency = persistency::destroying; m_decay = decay_timer; }
			unsigned int decay_timer() const { return m_decay; }
			void decay() { if (m_decay > 0) --m_decay; }
			void decay(unsigned int time) { m_decay -= m_decay > time ? time : m_decay; }

		public:
			unit() : m_persistency(persistency::serialized), m_location(nullptr), m_decay(0) {}

		private:
			persistency m_persistency;
			location_component* m_location;
			unsigned int m_decay;
		};
	}
}

#endif