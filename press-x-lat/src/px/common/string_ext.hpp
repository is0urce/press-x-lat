// name: string_ext.hpp
// type: c++ header
// desc: string functions
// auth: is0urce

#ifndef PX_COMMON_STRING_EXT_HPP
#define PX_COMMON_STRING_EXT_HPP

#include <px/common/point.hpp>
#include <px/common/vector.hpp>

#include <string>

namespace px
{
	namespace
	{
		std::string to_string(point2 const& p)
		{
			return "[" + std::to_string(p.get<0>()) + ":" + std::to_string(p.get<1>()) + "]";
		}

		std::string to_string(vector2 const& p)
		{
			return "[" + std::to_string(p.get<0>()) + ":" + std::to_string(p.get<1>()) + "]";
		}
	}
}

#endif