// name: image.hpp
// type: c++ header
// desc: class declaration
// auth: is0urce

// pod for location component

#ifndef PX_CORE_LOCATION_HPP
#define PX_CORE_LOCATION_HPP

#include <px/common/point.hpp>

namespace px
{
	namespace core
	{
		struct location
		{
		public:
			point2 position;
		};
	}
}

#endif