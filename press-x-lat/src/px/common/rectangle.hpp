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
		bool empty() const
		{
			return m_range.x() <= 0 && m_range.y() <= 0;
		}

		bool contains(point2 const& point) const
		{
			return point.x() >= m_start.x() && point.y() >= m_start.y() && point.x() < m_corner.x() && point.y() < m_corner.y();
		}
		bool contains(rectangle const& rect) const
		{
			return rect.m_start.x() >= m_start.x()
				&& rect.m_start.y() >= m_start.y()
				&& rect.m_corner.x() <= m_corner.x()
				&& rect.m_corner.y() <= m_corner.y();
		}
		bool is_border(point2 const& point) const
		{
			if (m_range.x() <= 0 || m_range.y() <= 0) return false;
			return point.x() == m_start.x() || point.y() == m_start.y() || point.x() == m_corner.x() || point.y() == m_corner.y();
		}
		rectangle intersection(rectangle const& with) const
		{
			auto start_x = (std::max)(m_start.x(), with.m_start.x());
			auto start_y = (std::max)(m_start.y(), with.m_start.y());
			auto corner_x = (std::min)(m_corner.x(), with.m_corner.x());
			auto corner_y = (std::min)(m_corner.y(), with.m_corner.y());
			return rectangle({ start_x, start_y }, { corner_x - start_x, corner_y - start_y });
		}

		// enumerate rectangle points from start to start + range
		template <typename Operator>
		void enumerate(Operator&& fn) const
		{
			for (int j = m_start.y(); j < m_corner.y(); ++j)
			{
				for (point2 cursor(m_start.x(), j); cursor.x() < m_corner.x(); cursor.increment_axis<0>())
				{
					std::forward<Operator>(fn)(cursor);
				}
			}
		}
		template <typename Operator>
		void enumerate_border(Operator&& fn) const
		{
			if (m_range.x() == 1)
			{
				for (int i = m_start.x(), j = m_start.y(); j < m_corner.y(); ++j)
				{
					std::forward<Operator>(fn)(point2(i, j));
				}
			}
			else if (m_range.y() == 1)
			{
				for (int i = m_start.x(), j = m_start.y(); i < m_corner.x(); ++i)
				{
					std::forward<Operator>(fn)(point2(i, j));
				}
			}
			else if (m_range.x() > 0 && m_range.y() > 0)
			{
				for (int i = m_start.x(), j = m_start.y(); i < m_corner.x(); ++i)
				{
					std::forward<Operator>(fn)(point2(i, j));
				}
				for (int i = m_corner.x() - 1, j = m_start.y() + 1; j < m_corner.y(); ++j)
				{
					std::forward<Operator>(fn)(point2(i, j));
				}
				for (int i = m_corner.x() - 1, j = m_corner.y() - 1; i > m_start.x(); --i)
				{
					std::forward<Operator>(fn)(point2(i - 1, j));
				}
				for (int i = m_start.x(), j = m_corner.y() - 1; j > m_start.y() + 1; --j)
				{
					std::forward<Operator>(fn)(point2(i, j - 1));
				}
			}
		}

		void inflate(int size)
		{
			m_start.move(point2(-size, -size));
			m_range.move(point2(size * 2, size * 2));
			m_corner.move(point2(size, size));
		}
		void inflate()
		{
			m_start.move(point2(-1, -1));
			m_range.move(point2(2, 2));
			m_corner.move(point2(1, 1));
		}
		void deflate(int size)
		{
			inflate(-size);
		}
		void deflate()
		{
			inflate(-1);
		}

		rectangle inflated(int size) const
		{
			rectangle result = *this;
			result.inflate(size);
			return result;
		}
		rectangle deflated(int size) const
		{
			rectangle result = *this;
			result.deflate(size);
			return result;
		}
	};

	inline bool operator==(px::rectangle const& a, px::rectangle const& b)
	{
		return a.range() == b.range() && a.start() == b.start();
	}
	inline bool operator!=(px::rectangle const& a, px::rectangle const& b)
	{
		return !operator==(a, b);
	}
}

#endif