// name: pool.hpp
// type: c++ header
// desc: memory pool with templated specialization
// auth: is0urce

#pragma once

// cashe coherent (or at least sequental) enumeration of living objects with On = elements allocated
// constant request() and release() methods
// for general performanse there is no internal support for concurrent requests and/or releases

#include <array>

namespace px
{
	template <typename T, size_t Size>
	class pool
	{
	public:
		struct record
		{
			T element;
			record* next_free;
			record* prev_free;
			record* next_live;
			record* prev_live;
			bool live;
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
		T* request() noexcept
		{
			record* rec = m_free;
			if (rec == nullptr) return nullptr;

			// update free ranges
			m_free = m_free->next_free;

			// update links
			if (rec->prev_live)
			{
				rec->prev_live->next_live = rec;
			}
			else // start or continue living queue
			{
				if (m_live != nullptr)
				{
					m_live->prev_live = rec;
					rec->next_live = m_live;
				}
				m_live = rec;
			}
			if (rec->next_live)
			{
				rec->next_live->prev_live = rec;
			}
			if (rec->prev_free)
			{
				rec->prev_free->next_free = rec->next_free;
			}
			if (rec->next_free)
			{
				rec->next_free->prev_free = rec->prev_free;
			}

			rec->live = true;
			++m_current;
			return &(rec->element);
		}
		void release(T* ptr) noexcept
		{
			record* rec = &m_pool[(ptr - &m_pool[0].element) / sizeof(record)];

			if (rec->live) // ensure it's not double release
			{
				// wip

				rec->live = false;
				--m_current;
			}
		}
		template <typename Operator>
		void enumerate_active(Operator && op)
		{
			for (record* i = m_live; i != nullptr; i = i->next_live)
			{
				std::forward<Operator>(op)(i->element);
			}
		}
		void clear() noexcept
		{
			m_current = 0;
			m_free = &m_pool[0];
			m_live = nullptr;
			for (size_t i = 0; i != Size; ++i)
			{
				m_pool[i].prev_free = i == 0 ? nullptr : &m_pool[i - 1];
				m_pool[i].next_free = i + 1 != Size ? nullptr : &m_pool[i + 1];
				m_pool[i].prev_live = nullptr;
				m_pool[i].next_live = nullptr;
			}
		}

	public:
		pool() noexcept
		{
			clear();
		}

	private:
		std::array<record, Size> m_pool;
		size_t m_current;
		record* m_free; // first free node
		record* m_live; // first living node
	};
}