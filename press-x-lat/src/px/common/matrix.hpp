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
	template <typename Element , unsigned int...> class matrix2;

	// matrix with fixed sizes

	template <typename Element, unsigned int W, unsigned int H>
	class matrix2<Element, W, H>
	{
	public:
		typedef Element element;

	private:
		std::array<element, W * H> m_data;

	public:
		matrix2(const matrix2&) = delete;
		matrix2() {}
		matrix2(const element &initial)
		{
			fill(initial);
		}
		template <typename Generator>
		matrix2(Generator op)
		{
			fill(std::forward<Generator>(op));
		}

		unsigned int width() const
		{
			return W;
		}
		unsigned int height() const
		{
			return H;
		}
		unsigned int size() const
		{
			return W * H;
		}
		void swap(matrix2 &that)
		{
			std::swap(m_data, that.m_data);
		}
		void copy(const matrix2 &that)
		{
			size_t len = W * H;
			for (size_t i = 0; i < len; ++i)
			{
				m_data[i] = that.m_data[i];
			}
		}

		bool contains(coordinate<int, 2> position) const
		{
			return contains(position.get<0>(), position.get<1>());
		}
		bool contains(point2 position) const
		{
			return contains(position.x(), position.y());
		}
		bool contains(unsigned int x, unsigned int y) const
		{
			return x >= 0 && x < W && y >= 0 && y < H;
		}

		void fill(const element &e)
		{
			m_data.fill(e);
		}
		template <typename Generator>
		void fill(Generator op)
		{
			size_t index = 0;
			for (unsigned int j = 0; j < H; ++j)
			{
				for (unsigned int i = 0; i < W; ++i)
				{
					m_data[index] = op(i, j);
					++index;
				}
			}
		}
		template <typename Operator>
		void enumerate(Operator&& op)
		{
			size_t index = 0;
			for (unsigned int j = 0; j < H; ++j)
			{
				for (unsigned int i = 0; i < W; ++i)
				{
					std::forward<Operator>(op)(i, j, m_data[index]);
					++index;
				}
			}
		}

		// querry functions: operator[] not throws, at() throws, select returns default (outer) if out of range
		// specialized point2 acessors for easy querry with bracket-initialized points
		template<typename Component>
		const element& operator[](coordinate<Component, 2> key) const
		{
			return m_data[key.get<1>() * W + key.get<0>()];
		}
		template<typename Component>
		element& operator[](coordinate<Component, 2> key)
		{
			return m_data[key.get<1>() * W + key.get<0>()];
		}
		const element& operator[](point2 key) const
		{
			return m_data[key.y() * W + key.x()];
		}
		element& operator[](point2 key)
		{
			return m_data[key.y() * W + key.x()];
		}

		template<typename Component>
		const element& at(coordinate<Component, 2> key) const
		{
			if (!contains(key)) throw std::runtime_error("px::matrix<e,w,h>::at() - out of range");
			return operator[](key);
		}
		template<typename Component>
		element& at(coordinate<Component, 2> key)
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
			return m_data[y * W + x];
		}
		element& at(unsigned int x, unsigned int y)
		{
			if (!contains(x, y)) throw std::runtime_error("px::matrix<e,w,h>::at() - out of range");
			return m_data[y * W + x];
		}

		template<typename Component>
		const element& select(coordinate<Component, 2> key, const element& outer) const
		{
			return contains(key) ? operator[](key) : outer;
		}
		template<typename Component>
		element& select(coordinate<Component, 2> key, element& outer)
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
			return contains(x, y) ? m_data[y * W + x];
		}
		element& select(unsigned int x, unsigned int y, element& outer)
		{
			return contains(x, y) ? m_data[y * W + x];
		}
	};

	// matrix with varying size

	template <typename Element>
	class matrix2<Element>
	{
	public:
		typedef Element element;
	private:
		std::vector<element> m_data;
		unsigned int m_width;
		unsigned int m_height;

	public:
		matrix2() : m_width(0), m_height(0)
		{
		}
		matrix2(const matrix2&) = delete;
		matrix2(matrix2&& that) : matrix2()
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
		template <typename Generator>
		matrix2(unsigned int w, unsigned int h, Generator op) : matrix2(w, h)
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
		point2 range() const
		{
			return point2(m_width, m_height);
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

		void fill(const element &e)
		{
			unsigned int len = size();
			for (unsigned int i = 0; i < len; ++i)
			{
				m_data[i] = e;
			}
		}
		template <typename Generator>
		void fill(Generator op)
		{
			size_t index = 0;
			for (unsigned int j = 0; j < m_height; ++j)
			{
				for (unsigned int i = 0; i < m_width; ++i)
				{
					m_data[index] = op(i, j);
					++index;
				}
			}
		}
		template <typename Operator>
		void enumerate(Operator &&op)
		{
			size_t index = 0;
			for (unsigned int j = 0; j < m_height; ++j)
			{
				for (unsigned int i = 0; i < m_width; ++i)
				{
					std::forward<Operator>(op)(i, j, m_data[index]);
					++index;
				}
			}
		}

		// querry functions: operator[] not throws, at() throws, select returns default (outer) if out of range
		// specialized point2 acessors for easy querry with bracket-initialized points

		template<typename Component>
		const element& operator[](coordinate<Component, 2> key) const
		{
			return m_data[key.get<1>() * m_width + key.get<0>()];
		}
		template<typename Component>
		element& operator[](coordinate<Component, 2> key)
		{
			return m_data[key.get<1>() * m_width + key.get<0>()];
		}
		const element& operator[](point2 key) const
		{
			return m_data[key.y() * m_width + key.x()];
		}
		element& operator[](point2 key)
		{
			return m_data[key.y() * m_width + key.x()];
		}

		template<typename Component>
		const element& at(coordinate<Component, 2> key) const
		{
			if (!contains(key)) throw std::runtime_error("px::matrix<e>::at() - out of range");
			return (*this)[key];
		}
		template<typename Component>
		element& at(coordinate<Component, 2> key)
		{
			if (!contains(key)) throw std::runtime_error("px::matrix<e>::at() - out of range");
			return (*this)[key];
		}
		const element& at(point2 key) const
		{
			if (!contains(key)) throw std::runtime_error("px::matrix<e>::at() - out of range");
			return (*this)[key];
		}
		element& at(point2 key)
		{
			if (!contains(key)) throw std::runtime_error("px::matrix<e>::at() - out of range");
			return (*this)[key];
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

		template<typename Component>
		const element& select(coordinate<Component, 2> key, const element& outer) const
		{
			return contains(key) ? (*this)[key] : outer;
		}
		template<typename Component>
		element& select(coordinate<Component, 2> key, element& outer)
		{
			return contains(key) ? (*this)[key] : outer;
		}
		const element& select(point2 key, const element& outer) const
		{
			return contains(key) ? (*this)[key] : outer;
		}
		element& select(point2 key, element& outer)
		{
			return contains(key) ? (*this)[key] : outer;
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