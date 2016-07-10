// name: alignment.hpp
// type: c++
// desc: struct declaration
// auth: is0urce

#ifndef PX_UI_ALIGNMENT_HPP
#define PX_UI_ALIGNMENT_HPP

#include <px/common/point.hpp>
#include <px/common/vector.hpp>
#include <px/common/rectangle.hpp>

namespace px
{
	namespace ui
	{
		struct alignment
		{
		public:
			vector2 anchor_percent;
			point2 anchor_offset;
			point2 size_absolute;
			vector2 size_relative;

			alignment()
			{
			}
			alignment(vector2 a_percent, point2 a_offset, point2 s_absolute, vector2 s_relative)
				: anchor_percent(a_percent)
				, anchor_offset(a_offset)
				, size_absolute(s_absolute)
				, size_relative(s_relative)
			{
			}

			rectangle calculate(const rectangle &parent) const
			{
				point2 start = anchor_offset + parent.start() + (anchor_percent * parent.range()).ceil();
				point2 range = size_absolute + (size_relative * parent.range()).ceil();
				return { start, range };
			}
			rectangle calculate() const
			{
				return { anchor_offset, size_absolute };
			}
		};
	}
}

#endif