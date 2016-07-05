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

	// matrix with fixed sizes

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
		template <typename _O>
		matrix2(_O op)
		{
			if (!op) throw std::runtime_error("px::common::matrix2<e,w,h>::ctor(op) - op is null");
			fill(op);
		}

		void swap(matrix2 &that)
		{
			std::swap(m_data, that.m_data);
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
		// specialized point acessors for easy querry with bracket-initialized points
		const element& operator[](point2 key) const
		{
			return m_data[key[0] * _W + key[1]];
		}
		element& operator[](point2 key)
		{
			return m_data[key[0] * _W + key[1]];
		}

		void fill(element e)
		{
			for (unsigned int i = 0; i < size; ++i)
			{
				m_data[i] = e;
			}
		}
		template <typename _O>
		void fill(_O op)
		{
			if (!op) throw std::runtime_error("px::common::matrix2<e,w,h>::fill(op) - op is null");

			unsigned int index = 0;
			for (unsigned int j = 0; j < _H; ++j)
			{
				for (unsigned int i = 0; i < _W; ++i)
				{
					m_data[index] = op(i, j);
					++index;
				}
			}
		}
		template <typename _O>
		void enumerate(_O op)
		{
			if (!op) throw std::runtime_error("px::common::matrix2<e,w,h>::enumerate(op) - op is null");

			unsigned int index = 0;
			for (unsigned int j = 0; j < _H; ++j)
			{
				for (unsigned int i = 0; i < _W; ++i)
				{
					op(i, j, m_data[index]);
					++index;
				}
			}
		}
	};

	// matrix with varying size

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
		matrix2(unsigned int w, unsigned int h, element initial)
		{
			if (w == 0 && h == 0) throw std::runtime_error("px::common::matrix2::ctor(w, h, int) - size = 0");
			m_width = w;
			m_height = h;
			m_data.assign(w * h, initial);
		}
		template <typename _O>
		matrix2(unsigned int w, unsigned int h, _O op)
		{
			if (w == 0 && h == 0) throw std::runtime_error("px::common::matrix2<e>::ctor(w, h, operator) - size = 0");
			if (!op) throw std::runtime_error("px::common::matrix2<e>::ctor(w, h, operator) - op is null");

			m_width = w;
			m_height = h;
			m_data.resize(w * h);
			fill(op);
		}
		void resize(unsigned int w, unsigned int h)
		{
			if (w == 0 && h == 0) throw std::runtime_error("px::common::matrix2::resize(w, h) - size = 0");
			m_width = w;
			m_height = h;
			m_data.resize(w * h);
		}

		void swap(matrix2 &that)
		{
			std::swap(m_height, that.m_height);
			std::swap(m_width, that.m_width);
			std::swap(m_data, that.m_data);
		}

		unsigned int width()
		{
			return m_width;
		}
		unsigned int height()
		{
			return m_height;
		}
		unsigned int size()
		{
			return m_width * m_height;
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

		// specialized poin2 acessors for easy querry with bracket-initialized points

		const element& operator[](point2 key) const
		{
			return m_data[key[0] * m_width + key[1]];
		}
		element& operator[](point2 key)
		{
			return m_data[key[0] * m_width + key[1]];
		}

		void fill(element e)
		{
			unsigned int len = size();
			for (unsigned int i = 0; i < len; ++i)
			{
				m_data[i] = e;
			}
		}
		template <typename _O>
		void fill(_O op)
		{
			if (!op) throw std::runtime_error("px::common::matrix2<e>::fill(op) - op is null");

			unsigned int index = 0;
			for (unsigned int j = 0; j < m_height; ++j)
			{
				for (unsigned int i = 0; i < m_width; ++i)
				{
					m_data[index] = op(i, j);
					++index;
				}
			}
		}
		template <typename _O>
		void enumerate(_O op)
		{
			if (!op) throw std::runtime_error("px::common::matrix2<e>::enumerate(op) - op is null");

			unsigned int index = 0;
			for (unsigned int j = 0; j < m_height; ++j)
			{
				for (unsigned int i = 0; i < m_width; ++i)
				{
					op(i, j, m_data[index]);
					++index;
				}
			}
		}
	};
}

#endif