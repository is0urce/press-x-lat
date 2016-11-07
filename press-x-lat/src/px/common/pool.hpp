// name: pool.hpp
// type: c++ header
// desc: template class
// auth: is0urce

#pragma once

// object memory pool
// cashe coherent (or at least sequental) enumeration of living objects with On = elements currently allocated
// constant request() and release() methods
// for general performanse there is no internal support for concurrent requests and/or releases

#include <array>

namespace px
{
	template <typename T, size_t Size>
	class pool
	{
	public:
		struct links
		{
			links* next_free;
			links* next_live;
			links* prev_live;
			bool live; // auxiliary state to avoid double released
		};
	public:
		size_t size() const noexcept
		{
			return m_current;
		}
		bool full() const noexcept
		{
			return m_current == Size;
		}
		bool empty() const noexcept
		{
			return m_current == 0;
		}

		// returns nullptr if all object in pool were requested, full() returns true
		T* request()
		{
			T* result = nullptr;
			links* rec = m_free;
			if (rec != nullptr)
			{
				m_free = m_free->next_free; // pop free stack
				m_live = rec->prev_live == nullptr ? rec : m_live; // set this as root?

				// update links
				if (rec->next_live)
				{
					rec->next_live->prev_live = rec;
				}
				if (rec->prev_live)
				{
					rec->prev_live->next_live = rec;
				}

				// modify aux fields
				rec->live = true;
				++m_current;

				result = reinterpret_cast<T*>(&m_pool[(rec - &m_links[0]) * sizeof(T)]);
				new (result) T;
			}
			return result;
		}

		// pointer must be in correct range and alignment of pool
		// it's safe to release already released objects - it's nop
		bool release(T* ptr)
		{
			bool flag = false;
			auto index = ptr - reinterpret_cast<T*>(&m_pool[0]);
			if (index >= 0 && index < Size)
			{
				if (m_links[index].live) // ensure it's not double release
				{
					flag = true;
					links* rec = &m_links[index];

					rec->next_free = m_free;
					m_free = rec; // push free stack
					m_live = m_live == rec ? rec->next_live : m_live; // don't be root anymore

					// update links
					if (rec->prev_live)
					{
						rec->prev_live->next_live = rec->next_live;
					}
					if (rec->next_live)
					{
						rec->next_live->prev_live = rec->prev_live;
					}

					// modify aux fields
					rec->live = false;
					--m_current;

					ptr->~T();
				}
			}
			return flag;
		}
		// checks only range of pointer, not correctness (i.e alignment)
		bool contains(T const* ptr) const noexcept
		{
			return ptr >= reinterpret_cast<T const*>(&m_pool[0])
				&& ptr < reinterpret_cast<T const*>(&m_pool[Size * sizeof(T)]);
		}

		template <typename Operator>
		void enumerate(Operator && op)
		{
			for (links* i = m_live; i != nullptr; i = i->next_live)
			{
				std::forward<Operator>(op)(reinterpret_cast<T&>(m_pool[(i - &m_links[0]) * sizeof(T)]));
			}
		}

		void clear() noexcept
		{
			for (links* i = m_live; i != nullptr; i = i->next_live)
			{
				reinterpret_cast<T&>(m_pool[(i - &m_links[0]) * sizeof(T)]).~T();
			}
			startup();
		}
		void swap(pool & xrh) noexcept
		{
			std::swap(m_pool, xrh.m_pool);
			std::swap(m_current, xrh.m_current);
			std::swap(m_free, xrh.m_free);
			std::swap(m_live, xrh.m_live);
		}

	public:
		pool() noexcept
		{
			startup();
		}
		pool(pool const&) = delete;
		pool& operator=(pool const&) = delete;
		pool(pool && xrh) noexcept
			: pool()
		{
			swap(xrh);
		}
		pool& operator=(pool && xrh) noexcept
		{
			swap(xrh);
			return *this;
		}

	private:
		void startup() noexcept
		{
			m_current = 0;
			m_free = &m_links[0];
			m_live = nullptr;
			for (size_t i = 0; i != Size; ++i)
			{
				m_links[i].next_free = i == Size - 1 ? nullptr : &m_links[i + 1];
				m_links[i].prev_live = i == 0 ? nullptr : &m_links[i - 1];
				m_links[i].next_live = nullptr;
				m_links[i].live = false;
			}
		}

	private:
		std::array<char, sizeof(T) * Size> m_pool;
		std::array<links, Size> m_links;
		size_t m_current; // cashed number of living objest for fast size queries
		links* m_free; // first free node (root)
		links* m_live; // first living node (root)
	};
}