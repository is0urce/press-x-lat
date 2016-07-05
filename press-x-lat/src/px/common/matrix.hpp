// name: coordinate.hpp
// type: c++ header
// desc: base template class for points and vectors

#ifndef PX_COMMON_ARRAY_BASE_HPP
#define PX_COMMON_ARRAY_BASE_HPP

#include <array>
#include <vector>
#include <stdexcept>

#include "point.hpp"

namespace px
{
	template <typename _E, unsigned int...> class matrix2;

	template <typename _E, unsigned int _W, unsigned int _H>
	class matrix2<_E, _W, _H>
	{
	public:
		typedef _E element;
		const static unsigned int width = _W;
		const static unsigned int height = _H;

		const static unsigned int size = _W * _H;
	private:
		std::array<element, size> m_data;

	public:
		matrix2(const matrix2&) = delete;
		matrix2() {}
		matrix2(element initial)
		{
			m_data.fill(initial);
		}

		void swap(matrix2 &that)
		{
			std::swap(m_data, that.m_data);
		}

		const element& operator[](point2 key) const
		{
			return m_data[key[0] * _W + key[1]];
		}
		element& operator[](point2 key)
		{
			return m_data[key[0] * _W + key[1]];
		}
		template<typename _C>
		const element& operator[](coordinate<_C, 2> key) const
		{
			return m_data[key[0] * _W + key[1]];
		}
		template<typename _C>
		element& operator[](coordinate<_C, 2> key)
		{
			return m_data[key[0] * _W + key[1]];
		}
	};

	template <typename _E>
	class matrix2<_E>
	{
	public:
		typedef _E element;
	private:
		std::vector<element> m_data;
		unsigned int m_width;
		unsigned int m_height;

	public:
		matrix2(const matrix2&) = delete;
		matrix2(unsigned int w, unsigned int h)
		{
			if (w == 0 && h == 0) throw std::runtime_error("px::common::matrix2::ctor(w, h) - size = 0");
			m_width = w;
			m_height = h;
			m_data.resize(w * h);
		}
		matrix2(unsigned int w, unsigned int h, element initial) : matrix2(w, h)
		{
			if (w == 0 && h == 0) throw std::runtime_error("px::common::matrix2::ctor(w, h, int) - size = 0");
			m_width = w;
			m_height = h;
			m_data.assign(w * h, initial);
		}

		void swap(matrix2 &that)
		{
			std::swap(m_height, that.m_height);
			std::swap(m_width, that.m_width);
			std::swap(m_data, that.m_data);
		}

		const element& operator[](point2 key) const
		{
			return m_data[key[0] * m_width + key[1]];
		}
		element& operator[](point2 key)
		{
			return m_data[key[0] * m_width + key[1]];
		}
		template<typename _C, unsigned int D>
		const element& operator[](coordinate<int, 2> key) const
		{
			return m_data[key[0] * m_width + key[1]];
		}
		template<typename _C, unsigned int D>
		element& operator[](coordinate<int, 2> key)
		{
			return m_data[key[0] * m_width + key[1]];
		}
	};
}

#endif