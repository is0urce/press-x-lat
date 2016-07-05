// name: coordinate.hpp
// type: c++ header
// desc: base template class for points and vectors

#ifndef PX_COORDINATE_HPP
#define PX_COORDINATE_HPP

#include <algorithm>
#include <array>

namespace px
{
	template <typename _C, unsigned int _D>
	struct coordinate
	{
	public:
		typedef _C component;
		static const unsigned int depth = _D;
	protected:
		std::array<component, _D> m_array;

	public:
		const component& operator[](unsigned int i) const
		{
			return m_array[i];
		}
		component& operator[](unsigned int i)
		{
			return m_array[i];
		}

		// transforms

		void negate()
		{
			for (unsigned int i = 0; i < _D; ++i)
			{
				m_array[i] = -m_array[i];
			}
		}
		void move(const coordinate &move)
		{
			for (unsigned int i = 0; i < _D; ++i)
			{
				m_array[i] += move[i];
			}
		}
		void reverse(const coordinate &move)
		{
			for (unsigned int i = 0; i < _D; ++i)
			{
				m_array[i] -= move[i];
			}
		}
		void multiply(const coordinate &multiplier)
		{
			for (unsigned int i = 0; i < _D; ++i)
			{
				m_array[i] *= multiplier[i];
			}
		}
		void multiply(component multiplier)
		{
			for (unsigned int i = 0; i < _D; ++i)
			{
				m_array[i] *= multiplier;
			}
		}
		void divide(const coordinate &divisor)
		{
			for (unsigned int i = 0; i < _D; ++i)
			{
				m_array[i] /= divisor[i];
			}
		}
		void divide(component divisor)
		{
			for (unsigned int i = 0; i < _D; ++i)
			{
				m_array[i] /= divisor;
			}
		}

		// distances

		// chebyshev distance to specified coordinate
		component king_distance(const coordinate &target) const
		{
			component distance{};
			for (unsigned int i = 0; i < _D; ++i)
			{
				distance = (std::max)(distance, (std::abs)(m_array[i] - target.m_array[i]));
			}
			return distance;
		}

		// manhattan distance to specified coordinate
		component block_distance(const coordinate &target) const
		{
			component distance{};
			for (unsigned int i = 0; i < _D; ++i)
			{
				distance += (std::abs)(m_array[i] - target.m_array[i]);
			}
			return distance;
		}

		// distance to to specified coordinate, squared for performance reasons
		component distance2(const coordinate &target) const
		{
			component distance{};
			for (unsigned int i = 0; i < _D; ++i)
			{
				component delta = m_array[i] - target.m_array[i]
				distance += delta * delta;
			}
			return distance;
		}

		component magnitude2() const
		{
			component distance{};
			for (unsigned int i = 0; i < _D; ++i)
			{
				distance += m_array[i] * m_array[i];
			}
			return distance;
		}

		// distance to to specified coordinate
		auto distance(const coordinate &target) const -> decltype(std::sqrt(distance2(target)))
		{
			return std::sqrt(distance2(target));
		}

		// size of vector
		auto magnitude() -> decltype(std::sqrt(magnitude2())) const
		{
			return std::sqrt(magnitude2());
		}

		// i/o

		template <typename _M>
		void read(_M *memory)
		{
			for (unsigned int i = 0; i < _D; ++i)
			{
				m_array[i] = (_C)memory[i];
			}
		}
		template <typename _M>
		void write(_M *memory) const
		{
			for (unsigned int i = 0; i < _D; ++i)
			{
				memory[i] = (_M)m_array[i];
			}
		}
	};

	template <typename _C, unsigned int _D>
	inline bool operator==(const coordinate<_C, _D> &a, const coordinate<_C, _D> &b)
	{
		for (unsigned int i = 0; i < _D; ++i)
		{
			if (a[i] != b[i]) return false;
		}
		return true;
	}
	template <typename _C, unsigned int _D>
	inline bool operator!=(const coordinate<_C, _D> &a, const coordinate<_C, _D> &b)
	{
		return !operator==(a, b);
	}
}

#endif