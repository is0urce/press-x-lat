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
		matrix2(const element &initial)
		{
			fill(initial);
		}
		template <typename _O>
		matrix2(_O op)
		{
			//if (!op) throw std::runtime_error("px::common::matrix2<e,w,h>::ctor(op) - op is null");
			fill(op);
		}

		void swap(matrix2 &that)
		{
			std::swap(m_data, that.m_data);
		}

		bool contains(coordinate<int, 2> position) const
		{
			return contains(position[0], position[1]);
		}
		bool contains(point2 position) const
		{
			return contains(position.x(), position.y());
		}
		bool contains(unsigned int x, unsigned int y) const
		{
			return x >= 0 && x < _W && y >= 0 && y < _H;
		}

		void fill(const element &e)
		{
			m_data.fill(e);
		}
		template <typename _O>
		void fill(_O op)
		{
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

		// querry functions: operator[] not throws, at() throws, select returns default (outer) if out of range
		// specialized point2 acessors for easy querry with bracket-initialized points
		template<typename _C>
		const element& operator[](coordinate<_C, 2> key) const
		{
			return m_data[key[1] * _W + key[0]];
		}
		template<typename _C>
		element& operator[](coordinate<_C, 2> key)
		{
			return m_data[key[1] * _W + key[0]];
		}
		const element& operator[](point2 key) const
		{
			return m_data[key.y() * _W + key.x()];
		}
		element& operator[](point2 key)
		{
			return m_data[key.y() * _W + key.x()];
		}

		template<typename _C, unsigned int D>
		const element& at(coordinate<int, 2> key) const
		{
			if (!contains(key)) throw std::runtime_error("px::matrix<e,w,h>::at() - out of range");
			return operator[](key);
		}
		template<typename _C, unsigned int D>
		element& at(coordinate<int, 2> key)
		{
			if (!contains(key)) throw std::runtime_error("px::matrix<e,w,h>::at() - out of range");
			return operator[](key);
		}
		const element& at(point2 key) const
		{
			if (!contains(key)) throw std::runtime_error("px::matrix<e,w,h>::at(point) - position out of range");
			return operator[](key);
		}
		element& at(point2 key)
		{
			if (!contains(key)) throw std::runtime_error("px::matrix<e,w,h>::at(point) - out of range");
			return operator[](key);
		}
		const element& at(unsigned int x, unsigned int y) const
		{
			if (!contains(x, y)) throw std::runtime_error("px::matrix<e,w,h>::at() - out of range");
			return m_data[y * _W + x];
		}
		element& at(unsigned int x, unsigned int y)
		{
			if (!contains(x, y)) throw std::runtime_error("px::matrix<e,w,h>::at() - out of range");
			return m_data[y * _W + x];
		}

		template<typename _C, unsigned int D>
		const element& select(coordinate<int, 2> key, const element& outer) const
		{
			return contains(key) ? operator[](key) : outer;
		}
		template<typename _C, unsigned int D>
		element& select(coordinate<int, 2> key, element& outer)
		{
			return contains(key) ? operator[](key) : outer;
		}
		const element& select(point2 key, const element& outer) const
		{
			return contains(key) ? operator[](key) : outer;
		}
		element& select(point2 key, element& outer)
		{
			return contains(key) ? operator[](key) : outer;
		}
		const element& select(unsigned int x, unsigned int y, const element& outer) const
		{
			return contains(x, y) ? m_data[y * _W + x];
		}
		element& select(unsigned int x, unsigned int y, element& outer)
		{
			return contains(x, y) ? m_data[y * _W + x];
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
		matrix2(matrix2&& that) : m_width(0), m_height(0)
		{
			swap(that);
		}
		matrix2(unsigned int w, unsigned int h) : m_width(w), m_height(h)
		{
			m_data.resize(w * h);
		}
		matrix2(unsigned int w, unsigned int h, const element &initial) : m_width(w), m_height(h)
		{
			m_data.assign(w * h, initial);
		}
		template <typename _O>
		matrix2(unsigned int w, unsigned int h, _O op) : matrix2(w, h)
		{
			fill(op);
		}
		void resize(unsigned int w, unsigned int h)
		{
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

		unsigned int width() const
		{
			return m_width;
		}
		unsigned int height() const
		{
			return m_height;
		}
		unsigned int size() const
		{
			return m_width * m_height;
		}
		bool empty()
		{
			return m_width == 0 || m_height == 0;
		}
		bool contains(coordinate<int, 2> position) const
		{
			return contains(position[0], position[1]);
		}
		bool contains(point2 position) const
		{
			return contains(position.x(), position.y());
		}
		bool contains(unsigned int x, unsigned int y) const
		{
			return x >= 0 && x < m_width && y >= 0 && y < m_height;
		}

		void fill(element e)
		{
			m_data.assign(size(), e);
		}
		template <typename _O>
		void fill(_O op)
		{
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

		// querry functions: operator[] not throws, at() throws, select returns default (outer) if out of range
		// specialized point2 acessors for easy querry with bracket-initialized points

		template<typename _C, unsigned int D>
		const element& operator[](coordinate<int, 2> key) const
		{
			return m_data[key[1] * m_width + key[0]];
		}
		template<typename _C, unsigned int D>
		element& operator[](coordinate<int, 2> key)
		{
			return m_data[key[1] * m_width + key[0]];
		}
		const element& operator[](point2 key) const
		{
			return m_data[key.y() * m_width + key.x()];
		}
		element& operator[](point2 key)
		{
			return m_data[key.y() * m_width + key.x()];
		}

		template<typename _C, unsigned int D>
		const element& at(coordinate<int, 2> key) const
		{
			if (!contains(key)) throw std::runtime_error("px::matrix<e>::at() - out of range");
			return operator[](key);
		}
		template<typename _C, unsigned int D>
		element& at(coordinate<int, 2> key)
		{
			if (!contains(key)) throw std::runtime_error("px::matrix<e>::at() - out of range");
			return operator[](key);
		}
		const element& at(point2 key) const
		{
			if (!contains(key)) throw std::runtime_error("px::matrix<e>::at() - out of range");
			return operator[](key);
		}
		element& at(point2 key)
		{
			if (!contains(key)) throw std::runtime_error("px::matrix<e>::at() - out of range");
			return operator[](key);
		}
		const element& at(unsigned int x, unsigned int y) const
		{
			if (!contains(x, y)) throw std::runtime_error("px::matrix<e>::at() - out of range");
			return m_data[y * m_width + x];
		}
		element& at(unsigned int x, unsigned int y)
		{
			if (!contains(x, y)) throw std::runtime_error("px::matrix<e>::at() - out of range");
			return m_data[y * m_width + x];
		}

		template<typename _C, unsigned int D>
		const element& select(coordinate<int, 2> key, const element& outer) const
		{
			return contains(key) ? operator[](key) : outer;
		}
		template<typename _C, unsigned int D>
		element& select(coordinate<int, 2> key, element& outer)
		{
			return contains(key) ? operator[](key) : outer;
		}
		const element& select(point2 key, const element& outer) const
		{
			return contains(key) ? operator[](key) : outer;
		}
		element& select(point2 key, element& outer)
		{
			return contains(key) ? operator[](key) : outer;
		}
		const element& select(unsigned int x, unsigned int y, const element& outer) const
		{
			return contains(x, y) ? m_data[y * m_width + x];
		}
		element& select(unsigned int x, unsigned int y, element& outer)
		{
			return contains(x, y) ? m_data[y * m_width + x];
		}
	};
}

#endif