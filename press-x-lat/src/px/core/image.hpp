// name: image.hpp
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_CORE_IMAGE_HPP
#define PX_CORE_IMAGE_HPP

#include <px/common/color.hpp>

namespace px
{
	namespace core
	{
		struct image
		{
		public:
			unsigned int glyph;
			color tint;
			color bg;
		};
	}
}

#endif