// name: pool.hpp
// type: c++ header
// desc: memory pool template
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
			record* next_live;
			record* prev_live;
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
		T* request() noexcept
		{
			record* rec = m_free;
			if (rec != nullptr)
			{
				m_free = m_free->next_free; // increment free queue

				// update links
				if (rec->next_live)
				{
					rec->next_live->prev_live = rec;
				}
				if (rec->prev_live)
				{
					rec->prev_live->next_live = rec;
				}
				else // start or continue living queue
				{
					if (m_live != nullptr)
					{
						if (rec->next_live == nullptr)
						{
							rec->prev_live = &m_pool[(rec - &m_pool[0]) - 1];
							rec->prev_live->next_live = rec;
						}
					}
					else // m_live == nullptr
					{
						m_live = rec;
					}
				}

				// modify aux fields
				rec->live = true;
				++m_current;

				return &rec->element;
			}
			return nullptr;
		}

		// pointer must be in correct range of pool
		// it's safe to release already released objects
		void release(T* ptr) noexcept
		{
			record* rec = &m_pool[reinterpret_cast<record*>(ptr) - reinterpret_cast<record*>(&m_pool[0])];

			if (rec->live) // ensure it's not double release
			{
				// wip

				// update links
				if (rec->prev_live)
				{
					rec->prev_live->next_live = rec->next_live;
				}
				if (rec->next_live)
				{
					rec->next_live->prev_live = rec->prev_live;
				}

				// update roots
				rec->next_free = m_free;
				m_free = rec;
				m_live = m_live == rec ? rec->next_live : m_live;

				// modify aux fields
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
				m_pool[i].next_free = i == Size - 1 ? nullptr : &m_pool[i + 1];
				m_pool[i].prev_live = nullptr;
				m_pool[i].next_live = nullptr;
				m_pool[i].live = false;
			}
		}

	public:
		pool() noexcept
		{
			static_assert(Size > 0, "px::pool<T, Size> - Size > 0 required"); // common sense and simplifies many formulaes
			clear();
		}

	private:
		std::array<record, Size> m_pool;
		size_t m_current; // cashed number of living objest for fast size queries
		record* m_free; // first free node (root)
		record* m_live; // first living node (root)
	};
}