// name: river.hpp
// type: c++ header
// desc: struct declaration
// auth: is0urce

#ifndef PX_CORE_RIVER_HPP
#define PX_CORE_RIVER_HPP

#include <px/common/point.hpp>

namespace px
{
	namespace core
	{
		struct river
		{
			double size;
			point2 flow; // direction of flow x = horisontal, y = vertical
		};
	}
}

#endif