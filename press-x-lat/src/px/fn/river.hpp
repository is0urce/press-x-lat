// name: river.hpp
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_FN_RIVER_HPP
#define PX_FN_RIVER_HPP

#include <px/common/point.hpp>

namespace px
{
	namespace fn
	{
		struct river
		{
			double size;
			point2 flow; // direction of flow x = horisontal, y = vertical
		};
	}
}

#endif