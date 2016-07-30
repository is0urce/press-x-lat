// name: coordinate.hpp
// type: c++ header
// desc: class for 2d point with integer components

#ifndef PX_POINT_HPP
#define PX_POINT_HPP

#include "coordinate.hpp"

#include <string>

namespace px
{
	struct point2 : public coordinate<int, 2>
	{
	public:
		point2()
		{

		}
		point2(component x, component y)
		{
			m_array[0] = x;
			m_array[1] = y;
		}
		component x() const
		{
			return m_array[0];
		}
		component y() const
		{
			return m_array[1];
		}

		point2 moved(point2 move) const { move.move(*this); return move; }
		point2 moved(component x, component y) const { point2 result(x, y); result.move(*this); return result; }
		point2 multiplied(point2 stretch) const { stretch.multiply(*this); return stretch; }

		point2 operator-() const { point2 negated = *this; negated.negate(); return negated; }

		point2& operator+=(const point2 &rhs) { move(rhs); return *this; }
		point2& operator-=(const point2 &rhs) { reverse(rhs); return *this; }
		point2& operator*=(const point2 &rhs) { multiply(rhs); return *this; }
		point2& operator/=(const point2 &rhs) { divide(rhs); return *this; }

		point2& operator*=(component c) { multiply(c); return *this; };
		point2& operator/=(component c) { divide(c); return *this; };

		point2 clamped(const point2 &min, const point2 &max) const
		{
			point2 result;
			for (unsigned int i = 0; i < depth; ++i)
			{
				result[i] = (std::min)((std::max)(min[i], m_array[i]), max[i]);
			}
			return result;
		}
	};

	inline point2 operator+(point2 lhs, const point2 &rhs) { lhs += rhs; return lhs; }
	inline point2 operator-(point2 lhs, const point2 &rhs) { lhs -= rhs; return lhs; }
	inline point2 operator*(point2 lhs, const point2 &rhs) { lhs *= rhs; return lhs; }
	inline point2 operator/(point2 lhs, const point2 &rhs) { lhs /= rhs; return lhs; }
	inline point2 operator*(point2 lhs, point2::component c) { lhs *= c; return lhs; }
	inline point2 operator/(point2 lhs, point2::component c) { lhs /= c; return lhs; }

	inline std::string to_string(const point2 &p)
	{
		return "[" + std::to_string(p.get<0>()) + ":" + std::to_string(p.get<1>()) + "]";
	}
}

#endif