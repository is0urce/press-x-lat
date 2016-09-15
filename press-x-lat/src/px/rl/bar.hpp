// name: bar.hpp
// type: c++ header
// desc: template class declaration & implementation
// auth: is0urce

// resource bar

#ifndef PX_RL_BAR_HPP
#define PX_RL_BAR_HPP

namespace px
{
	namespace rl
	{
		template <typename T>
		struct bar
		{
		public:
			typedef T value_type;

		private:
			T m_current;
			T m_max;

			// ctor & dtor
		public:
			bar() : m_current{}, m_max{}
			{
			}
			bar(T max) : m_current(max), m_max(max)
			{
			}
			bar(T current, T max)
			{
				init(max, max);
			}
		private:
			void init(T current, T max)
			{
				m_max = max;
				m_current = std::min<T>(current, m_max);
			}

			// methods
		public:
			void set(T current_and_max)
			{
				init(current_and_max, current_and_max);
			}
			void set(T current, T max)
			{
				init(current, max);
			}
			T current() const
			{
				return m_current;
			}
			T maximum() const
			{
				return m_max;
			}
			void modify(T magnitude)
			{
				m_current = std::min<T>(m_current + magnitude, m_max);
			}
			void restore(T magnitude)
			{
				m_current = std::min<T>(m_current + magnitude, m_max);
			}
			void damage(T magnitude)
			{
				m_current -= magnitude;
			}
			void current(T current)
			{
				m_current = std::min<T>(current, m_max);
			}
			void maximum(T max)
			{
				m_max = max;
				m_current = std::min<T>(m_current, m_max);
			}
			bool empty() const
			{
				return m_current <= 0;
			}
			bool full() const
			{
				return m_current == m_max;
			}
			bar& operator=(T c)
			{
				set(c);
				return *this;
			}
			bar& operator+=(T c)
			{
				modify(c);
				return *this;
			}
			bar& operator-=(T c)
			{
				modify(-c);
				return *this;
			}
			operator value_type() const
			{
				return m_current;
			}
			operator bool() const
			{
				return m_current > 0;
			}
		};
	}
}

#endif