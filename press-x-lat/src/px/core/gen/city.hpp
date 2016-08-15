// name: city.hpp
// type: c++ header
// desc: struct declaration
// auth: is0urce

#ifndef PX_CORE_CITY_HPP
#define PX_CORE_CITY_HPP

#include <px/rl/entity.hpp>

namespace px
{
	namespace core
	{
		struct city : public rl::entity
		{
			point2 center_location;
		};
	}
}

#endif