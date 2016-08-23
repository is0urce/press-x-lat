// name: coordinate.hpp
// type: c++ header
// desc: class for 2d verctor with real components

#ifndef PX_VECTOR_HPP
#define PX_VECTOR_HPP

#include "coordinate.hpp"
#include "point.hpp"

#include <string>

namespace px
{
	struct vector2 : public coordinate<double, 2>
	{
	public:
		vector2()
		{
		}
		vector2(component x, component y)
		{
			m_array[0] = x;
			m_array[1] = y;
		}
		template <typename Component>
		vector2(const coordinate<Component, 2> cast)
		{
			m_array[0] = cast[0];
			m_array[1] = cast[1];
		}
		component x() const
		{
			return m_array[0];
		}
		component y() const
		{
			return m_array[1];
		}

		point2 floor() const
		{
			point2 result;
			for (unsigned int i = 0; i < depth; ++i)
			{
				result[i] = (point2::component)std::floor(m_array[i]);
			}
			return result;
		}
		point2 ceil() const
		{
			point2 result;
			for (unsigned int i = 0; i < depth; ++i)
			{
				result[i] = (point2::component)std::ceil(m_array[i]);
			}
			return result;
		}
		point2 round() const
		{
			point2 result;
			for (unsigned int i = 0; i < depth; ++i)
			{
				result[i] = (point2::component)std::round(m_array[i]);
			}
			return result;
		}
		template<typename TargetType, typename Converter>
		TargetType convert(Converter convert_fn) const
		{
			TargetType result;
			for (unsigned int i = 0; i < depth; ++i)
			{
				result[i] = convert_fn(m_array[i]);
			}
			return result;
		}

		// mutations

		vector2 moved(vector2 move) const { move.move(*this); return move; }
		vector2 multiplied(vector2 stretch) const { stretch.multiply(*this); return stretch; }
		vector2 multiplied(component stretch) const { vector2 result(*this); result.multiplied(stretch); return result; }

		vector2 operator-() const { vector2 negated = *this; negated.negate(); return negated; }

		template <typename _T>
		vector2& operator+=(const coordinate<_T, depth> &rhs) { move(rhs); return *this; }
		template <typename _T>
		vector2& operator-=(const coordinate<_T, depth> &rhs) { reverse(rhs); return *this; }
		template <typename _T>
		vector2& operator*=(const coordinate<_T, depth> &rhs) { multiply(rhs); return *this; }
		template <typename _T>
		vector2& operator/=(const coordinate<_T, depth> &rhs) { divide(rhs); return *this; }

		vector2& operator*=(component c) { multiply(c); return *this; };
		vector2& operator/=(component c) { divide(c); return *this; };

		void normalize()
		{
			auto len = magnitude();
			if (len > 0)
			{
				for (unsigned int i = 0; i < depth; ++i)
				{
					m_array[i] /= len;
				}
			}
			else
			{
				for (unsigned int i = 0; i < depth; ++i)
				{
					m_array[i] = 0;
				}
			}
		}
		vector2 normalized() { vector2 result(*this); result.normalize(); return result; }
		vector2 clamped(const vector2 &min, const vector2 &max) const
		{
			vector2 result;
			for (unsigned int i = 0; i < depth; ++i)
			{
				result[i] = (std::min)((std::max)(min[i], m_array[i]), max[i]);
			}
			return result;
		}
		vector2 lerp(vector2 b, component t) const
		{
			b.multiply(t);
			b.move(multiplied(1.0 - t));
			return b;
		}
	};

	template <typename _T>
	vector2 operator+(vector2 lhs, const coordinate<_T, 2> &rhs) { lhs += rhs; return lhs; }
	template <typename _T>
	vector2 operator-(vector2 lhs, const coordinate<_T, 2> &rhs) { lhs -= rhs; return lhs; }
	template <typename _T>
	vector2 operator*(vector2 lhs, const coordinate<_T, 2> &rhs) { lhs *= rhs; return lhs; }
	template <typename _T>
	vector2 operator/(vector2 lhs, const coordinate<_T, 2> &rhs) { lhs /= rhs; return lhs; }

	namespace
	{
		vector2 operator*(vector2 lhs, point2::component c) { lhs *= c; return lhs; }
		vector2 operator/(vector2 lhs, point2::component c) { lhs /= c; return lhs; }

		std::string to_string(const vector2 &p)
		{
			return "[" + std::to_string(p.get<0>()) + ":" + std::to_string(p.get<1>()) + "]";
		}
	}
}

#endif