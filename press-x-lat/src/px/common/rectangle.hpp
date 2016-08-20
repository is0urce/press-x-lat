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
		rectangle() noexcept : m_start{}, m_range{}, m_corner{} {}
		rectangle(point2 range) noexcept : m_start(0, 0), m_range(range), m_corner(range) {}
		rectangle(point2 start, point2 range) noexcept : m_start(start), m_range(range), m_corner(start + range) {}

	public:
		point2::component width() const noexcept
		{
			return m_range.x();
		}
		point2::component height() const noexcept
		{
			return m_range.y();
		}
		point2 range() const noexcept
		{
			return m_range;
		}
		point2 start() const noexcept
		{
			return m_start;
		}
		point2 corner() const noexcept
		{
			return m_corner;
		}
		unsigned int size() const noexcept
		{
			return m_range.x() * m_range.y();
		}
		bool empty() const noexcept
		{
			return m_range.x() <= 0 && m_range.y() <= 0;
		}
		unsigned int perimeter() const noexcept
		{
			if (m_range.x() <= 0 || m_range.y() <= 0) return 0;
			return (m_range.x() + m_range.y()) * 2;
		}

		bool contains(point2 const& point) const noexcept
		{
			return point.x() >= m_start.x() && point.y() >= m_start.y() && point.x() < m_corner.x() && point.y() < m_corner.y();
		}
		bool contains(rectangle const& rect) const noexcept
		{
			return rect.m_start.x() >= m_start.x()
				&& rect.m_start.y() >= m_start.y()
				&& rect.m_corner.x() <= m_corner.x()
				&& rect.m_corner.y() <= m_corner.y();
		}
		bool is_border(point2 const& point) const noexcept
		{
			if (m_range.x() <= 0 || m_range.y() <= 0) return false;
			return point.x() == m_start.x() || point.y() == m_start.y() || point.x() == m_corner.x() - 1 || point.y() == m_corner.y() - 1;
		}
		bool is_corner(point2 const& point) const noexcept
		{
			if (m_range.x() <= 0 || m_range.y() <= 0) return false;
			return (point.x() == m_start.x() || point.x() == m_corner.x() - 1) && (point.y() == m_start.y() || point.y() == m_corner.y() - 1);
		}
		rectangle intersection(rectangle const& with) const noexcept
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

		void inflate(int size) noexcept
		{
			m_start.move(point2(-size, -size));
			m_range.move(point2(size * 2, size * 2));
			m_corner.move(point2(size, size));
		}
		void inflate() noexcept
		{
			m_start.move(point2(-1, -1));
			m_range.move(point2(2, 2));
			m_corner.move(point2(1, 1));
		}
		void deflate(int size) noexcept
		{
			inflate(-size);
		}
		void deflate() noexcept
		{
			inflate(-1);
		}

		rectangle inflated(int size) const noexcept
		{
			rectangle result = *this;
			result.inflate(size);
			return result;
		}
		rectangle deflated(int size) const noexcept
		{
			rectangle result = *this;
			result.deflate(size);
			return result;
		}
	};

	inline bool operator==(px::rectangle const& a, px::rectangle const& b) noexcept
	{
		return a.range() == b.range() && a.start() == b.start();
	}
	inline bool operator!=(px::rectangle const& a, px::rectangle const& b) noexcept
	{
		return !operator==(a, b);
	}
	inline px::rectangle operator&(px::rectangle const& a, px::rectangle const& b) noexcept
	{
		return a.intersection(b);
	}
}

#endif