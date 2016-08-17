// name: coordinate.hpp
// type: c++ header
// desc: base template class for points and vectors

#ifndef PX_COORDINATE_HPP
#define PX_COORDINATE_HPP

#include <algorithm>
#include <array>

namespace px
{
	template <typename Component, size_t Dim>
	struct coordinate
	{
	public:
		static const size_t depth = Dim;
		typedef Component component;

	protected:
		std::array<component, depth> m_array;

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
		template <size_t Index>
		const component get() const
		{
			return m_array[Index];
		}
		template <size_t Index>
		const component at() const
		{
			return m_array.at(Index);
		}

		bool empty() const
		{
			bool result = true;
			for (size_t i = 0; i < Dim; ++i)
			{
				result &= m_array[i] == 0;
			}
			return result;
		}

		// transforms

		void negate()
		{
			for (size_t i = 0; i < Dim; ++i)
			{
				m_array[i] = -m_array[i];
			}
		}
		template <size_t Axis>
		void mirror()
		{
			m_array[Axis] = -m_array[Axis];
		}
		void mirror(size_t axis)
		{
			m_array[axis] = -m_array[axis];
		}
		template <size_t Axis>
		void move_axis(component distance)
		{
			m_array[Axis] += distance;
		}
		void move_axis(size_t axis, component distance)
		{
			m_array[axis] += distance;
		}
		template <size_t Axis>
		void increment_axis()
		{
			m_array[Axis] += 1;
		}

		// vector transforms

		template<typename ConvertElement>
		void move(const coordinate<ConvertElement, depth> &move)
		{
			for (size_t i = 0; i < Dim; ++i)
			{
				m_array[i] += move[i];
			}
		}
		template<typename ConvertElement>
		void reverse(const coordinate<ConvertElement, depth> &move)
		{
			for (size_t i = 0; i < Dim; ++i)
			{
				m_array[i] -= move[i];
			}
		}
		template<typename ConvertElement>
		void multiply(const coordinate<ConvertElement, depth> &multiplier)
		{
			for (size_t i = 0; i < Dim; ++i)
			{
				m_array[i] *= multiplier[i];
			}
		}
		template<typename ConvertElement>
		void divide(const coordinate<ConvertElement, depth> &divisor)
		{
			for (size_t i = 0; i < Dim; ++i)
			{
				m_array[i] /= divisor[i];
			}
		}

		// component transforms

		void multiply(component multiplier)
		{
			for (size_t i = 0; i < Dim; ++i)
			{
				m_array[i] *= multiplier;
			}
		}
		void divide(component divisor)
		{
			for (size_t i = 0; i < Dim; ++i)
			{
				m_array[i] /= divisor;
			}
		}

		// distances

		// chebyshev distance to specified coordinate
		component king_distance(const coordinate &target) const
		{
			component distance{};
			for (size_t i = 0; i < Dim; ++i)
			{
				distance = (std::max)(distance, (std::abs)(m_array[i] - target.m_array[i]));
			}
			return distance;
		}

		// manhattan distance to specified coordinate
		component block_distance(const coordinate &target) const
		{
			component distance{};
			for (size_t i = 0; i < Dim; ++i)
			{
				distance += (std::abs)(m_array[i] - target.m_array[i]);
			}
			return distance;
		}

		// distance to to specified coordinate, squared for performance reasons
		component distance2(const coordinate &target) const
		{
			component distance{};
			for (size_t i = 0; i < Dim; ++i)
			{
				component delta = m_array[i] - target.m_array[i];
				distance += delta * delta;
			}
			return distance;
		}

		component magnitude2() const
		{
			component distance{};
			for (size_t i = 0; i < Dim; ++i)
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

		template <typename Memory>
		void read(Memory *memory)
		{
			for (size_t i = 0; i < Dim; ++i)
			{
				m_array[i] = static_cast<Memory>(memory[i]);
			}
		}
		template <typename Memory>
		void write(Memory *memory) const
		{
			for (size_t i = 0; i < Dim; ++i)
			{
				memory[i] = static_cast<Memory>(m_array[i]);
			}
		}
	};

	template <typename Component, unsigned int Dim>
	bool operator==(const coordinate<Component, Dim> &a, const coordinate<Component, Dim> &b)
	{
		for (size_t i = 0; i < Dim; ++i)
		{
			if (a[i] != b[i]) return false;
		}
		return true;
	}
	template <typename Component, unsigned int Dim>
	bool operator!=(const coordinate<Component, Dim> &a, const coordinate<Component, Dim> &b)
	{
		return !operator==(a, b);
	}
}

#endif