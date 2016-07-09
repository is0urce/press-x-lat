// name: unit.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_CORE_UNIT_HPP
#define PX_CORE_UNIT_HPP

#include <px/common/toggle.hpp>
#include <px/es/component_collection.hpp>

namespace px
{
	namespace core
	{
		class unit : public es::component_collection
		{
		private:
			bool m_destroy;

		public:
			unit() : m_destroy(false) {}

			bool destroying() const { return m_destroy; }
			void destroy() { m_destroy = true; }
		};
	}
}

#endif