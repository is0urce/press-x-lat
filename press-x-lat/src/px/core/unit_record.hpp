// name: unit_record.hpp
// type: c++ header
// desc: struct definition
// auth: is0urce

#ifndef PX_CORE_UNIT_RECORD_HPP
#define PX_CORE_UNIT_RECORD_HPP

#include <px/common/point.hpp>
#include <memory>

namespace px
{
	namespace core
	{
		class unit;
		class location_component;

		struct unit_record
		{
		public:
			typedef std::shared_ptr<unit> unit_ptr;
			typedef std::shared_ptr<location_component> location_ptr;

			unit_ptr unit;
			location_ptr location;

			unit_record(unit_ptr u, location_ptr l) : unit(u), location(l) {} // for emplace construction
		};
	}
}

#endif