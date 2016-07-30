// name: coordinate.hpp
// type: c++ header
// desc: base template class for points and vectors

#ifndef PX_COORDINATE_HPP
#define PX_COORDINATE_HPP

#include <algorithm>
#include <array>

namespace px
{
	template <typename _C, size_t _D>
	struct coordinate
	{
	public:
		static const size_t depth = _D;
		typedef _C component;

	protected:
		std::array<component, _D> m_array;

	public:
		const component& operator[](size_t i) const
		{
			return m_array[i];
		}
		component& operator[](size_t i)
		{
			return m_array[i];
		}
		const component get(size_t i) const
		{
			return m_array[i];
		}
		const component at(size_t i) const
		{
			return m_array.at(i);
		}
		template <size_t _I>
		const component get() const
		{
			return m_array[_I];
		}
		template <size_t _I>
		const component at() const
		{
			return m_array.at(_I);
		}

		bool empty() const
		{
			bool result = true;
			for (size_t i = 0; i < _D; ++i)
			{
				result &= m_array[i] == 0;
			}
			return result;
		}

		// transforms

		void negate()
		{
			for (size_t i = 0; i < _D; ++i)
			{
				m_array[i] = -m_array[i];
			}
		}
		template <size_t _Axis>
		void mirror()
		{
			m_array[_Axis] = -m_array[_Axis];
		}
		void mirror(size_t axis)
		{
			m_array[axis] = -m_array[axis];
		}
		template <size_t _Axis>
		void move_axis(component distance)
		{
			m_array[_Axis] += distance;
		}
		void move_axis(size_t axis, component distance)
		{
			m_array[axis] += distance;
		}

		// vector transforms

		template<typename _S>
		void move(const coordinate<_S, depth> &move)
		{
			for (size_t i = 0; i < _D; ++i)
			{
				m_array[i] += move[i];
			}
		}
		template<typename _S>
		void reverse(const coordinate<_S, depth> &move)
		{
			for (size_t i = 0; i < _D; ++i)
			{
				m_array[i] -= move[i];
			}
		}
		template<typename _S>
		void multiply(const coordinate<_S, depth> &multiplier)
		{
			for (size_t i = 0; i < _D; ++i)
			{
				m_array[i] *= multiplier[i];
			}
		}
		template<typename _S>
		void divide(const coordinate<_S, depth> &divisor)
		{
			for (size_t i = 0; i < _D; ++i)
			{
				m_array[i] /= divisor[i];
			}
		}

		// component transforms

		void multiply(component multiplier)
		{
			for (size_t i = 0; i < _D; ++i)
			{
				m_array[i] *= multiplier;
			}
		}
		void divide(component divisor)
		{
			for (size_t i = 0; i < _D; ++i)
			{
				m_array[i] /= divisor;
			}
		}

		// distances

		// chebyshev distance to specified coordinate
		component king_distance(const coordinate &target) const
		{
			component distance{};
			for (size_t i = 0; i < _D; ++i)
			{
				distance = (std::max)(distance, (std::abs)(m_array[i] - target.m_array[i]));
			}
			return distance;
		}

		// manhattan distance to specified coordinate
		component block_distance(const coordinate &target) const
		{
			component distance{};
			for (size_t i = 0; i < _D; ++i)
			{
				distance += (std::abs)(m_array[i] - target.m_array[i]);
			}
			return distance;
		}

		// distance to to specified coordinate, squared for performance reasons
		component distance2(const coordinate &target) const
		{
			component distance{};
			for (size_t i = 0; i < _D; ++i)
			{
				component delta = m_array[i] - target.m_array[i];
				distance += delta * delta;
			}
			return distance;
		}

		component magnitude2() const
		{
			component distance{};
			for (size_t i = 0; i < _D; ++i)
			{
				distance += m_array[i] * m_array[i];
			}
			return distance;
		}

		// distance to to specified coordinate
		auto distance(const coordinate &target) const
		{
			return std::sqrt(distance2(target));
		}

		// size of vector
		auto magnitude() const
		{
			return std::sqrt(magnitude2());
		}

		// i/o

		template <typename _M>
		void read(_M *memory)
		{
			for (size_t i = 0; i < _D; ++i)
			{
				m_array[i] = static_cast<_M>(memory[i]);
			}
		}
		template <typename _M>
		void write(_M *memory) const
		{
			for (size_t i = 0; i < _D; ++i)
			{
				memory[i] = static_cast<_M>(m_array[i]);
			}
		}
	};

	template <typename _C, unsigned int _D>
	bool operator==(const coordinate<_C, _D> &a, const coordinate<_C, _D> &b)
	{
		for (size_t i = 0; i < _D; ++i)
		{
			if (a[i] != b[i]) return false;
		}
		return true;
	}
	template <typename _C, unsigned int _D>
	bool operator!=(const coordinate<_C, _D> &a, const coordinate<_C, _D> &b)
	{
		return !operator==(a, b);
	}
}

#endif