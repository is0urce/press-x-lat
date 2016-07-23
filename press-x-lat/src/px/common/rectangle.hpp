// name: rectangle.hpp
// type: c++ header
// desc: class with implementation
// auth: is0urce

// 2-d rectangle with integer components

#ifndef PX_COMMON_RECTANGLE_HPP
#define PX_COMMON_RECTANGLE_HPP

#include "point.hpp"

#include <functional>

namespace px
{
	struct rectangle
	{
	private:
		point2 m_start;
		point2 m_range;
		point2 m_corner;

	public:
		rectangle() : m_start{}, m_range{}, m_corner{} {}
		rectangle(point2 range) : m_start(0, 0), m_range(range), m_corner(range) {}
		rectangle(point2 start, point2 range) : m_start(start), m_range(range), m_corner(start + range) {}

	public:
		point2::component width() const
		{
			return m_range.x();
		}
		point2::component height() const
		{
			return m_range.y();
		}
		point2 range() const
		{
			return m_range;
		}
		point2 start() const
		{
			return m_start;
		}
		point2 corner() const
		{
			return m_corner;
		}
		unsigned int size() const
		{
			return m_range.x() * m_range.y();
		}

		bool contains(const point2 &point) const
		{
			return point.x() >= m_start.x() && point.y() >= m_start.y() && point.x() < m_corner.x() && point.y() < m_corner.y();
		}
		bool contains(const rectangle &rect)
		{
			return rect.m_start.x() >= m_start.x()
				&& rect.m_start.y() >= m_start.y()
				&& rect.m_corner.x() <= m_corner.x()
				&& rect.m_corner.y() <= m_corner.y();
		}
		bool is_border(const point2 &point) const
		{
			if (m_range.x() <= 0 || m_range.y() <= 0) return false;
			return point.x() == m_start.x() || point.y() == m_start.y() || point.x() == m_corner.x() || point.y() == m_corner.y();
		}
		rectangle intersection(const rectangle &with) const
		{
			auto start_x = (std::max)(m_start.x(), with.m_start.x());
			auto start_y = (std::max)(m_start.y(), with.m_start.y());
			auto corner_x = (std::min)(m_corner.x(), with.m_corner.x());
			auto corner_y = (std::min)(m_corner.y(), with.m_corner.y());
			return rectangle({ start_x, start_y }, { corner_x - start_x, corner_y - start_y });
		}

		// enumerate rectangle points from start to start + range
		template <typename _O>
		void enumerate(_O fn) const
		{
			for (int j = m_start.y(); j < m_corner.y(); ++j)
			{
				for (int i = m_start.x(); i < m_corner.x(); ++i)
				{
					fn(i, j);
				}
			}
		}
	};
	inline bool operator==(const rectangle &a, const rectangle &b)
	{
		return a.range() == b.range() && a.start() == b.start();
	}
	inline bool operator!=(const rectangle &a, const rectangle &b)
	{
		return !operator==(a, b);
	}
}

#endif