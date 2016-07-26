// name: manager.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

// object static array pool manager, used in component systems
// _C - component type
// _B - one batch size

#ifndef PX_ES_MANAGER_HPP
#define PX_ES_MANAGER_HPP

#include <array>
#include <list>
#include <memory>
#include <mutex>
#include <vector>

namespace px
{
	namespace es
	{
		template<typename _C, size_t _B>
		class manager
		{
		public:
			typedef _C element;
			typedef std::shared_ptr<element> element_ptr;
			typedef std::array<element, _B> container_t;
			struct batch
			{
			private:
				container_t m_elements;
				std::array<bool, _B> m_exist;
				std::list<size_t> m_recycle;
				size_t m_cursor = 0;

			public:
				batch()
				{
					m_exist.fill(false);
				}

			public:
				bool exists(size_t n) const
				{
					return m_exist[n];
				}
				size_t recycled() const
				{
					return m_recycle.size();
				}
				size_t count() const
				{
					return m_cursor - recycled();
				}
				size_t cursor() const
				{
					return m_cursor;
				}
				bool empty() const
				{
					return count() == 0;
				}
				bool full() const
				{
					return count() == _B;
				}
				void clear()
				{
					m_recycle.clear();
					m_cursor = 0;
				}
				void optimise()
				{
					if (count() == 0)
					{
						clear();
					}
				}
				const _C& operator[](size_t index) const
				{
					return m_elements[index];
				}
				_C& operator[](size_t index)
				{
					return m_elements[index];
				}
				const _C& select(size_t index) const
				{
					return m_elements[index];
				}
				_C& select(size_t index)
				{
					return m_elements[index];
				}
				size_t increment()
				{
					auto index = m_cursor;
					++m_cursor;

					m_exist[index] = true;
					return index;
				}
				void recycle(size_t index)
				{
					m_recycle.push_back(index);
					m_exist[index] = false;

					// optimise batch
					optimise();
				}
				size_t recycle()
				{
					// use position of recycled component
					auto it = m_recycle.end();
					auto index = *it;
					m_recycle.erase(it);
					m_exist[index] = true;
					return index;
				}
			};
			typedef std::list<batch> batch_t;
			typedef typename batch_t::iterator batch_it;
			struct key
			{
			public:
				batch_it batch;
				size_t cursor;
				key()
				{
				}
				key(batch_it batch_iterator, size_t index) : batch(batch_iterator), cursor(index)
				{
				}
			};

		private:
			batch_t m_batches;
			std::mutex m_mutex;
			size_t m_count = 0;

		public:
			virtual ~manager() {}
			manager() {}
			manager(const manager&) = delete;

		protected:
			virtual void element_allocated(element &e) {}
			virtual void element_released(element &e) {}

		public:
			// count created objects
			size_t count() const
			{
				return m_count;
			}

			std::shared_ptr<element> make()
			{
				key k = select();
				element &e = k.batch->select(k.cursor);
				element_allocated(e);

				return std::shared_ptr<element>(&e, [this, k](element* pointer) { destroy(k); element_released(*pointer); });
			}

			template<typename _O>
			void enumerate(_O fn)
			{
				for (auto it = m_batches.begin(), last = m_batches.end(); it != last; ++it)
				{
					size_t cursor = it->cursor();
					for (size_t i = 0; i < cursor; ++i)
					{
						if (it->exists(i))
						{
							fn((*it)[i]);
						}
					}
				}
			}

			// clear unused batches
			void optimise()
			{
				for (auto it = m_batches.begin(), last = m_batches.end(); it != last;)
				{
					it->optimise();
					if (it->count() == 0)
					{
						++it;
						m_batches.erase(it);
					}
					else
					{
						++it;
					}
				}
			}

		private:
			key select()
			{
				std::lock_guard<std::mutex> lock(m_mutex);

				// select batch
				auto it = m_batches.begin();
				auto last = m_batches.end();
				while (it != last && it->full())
				{
					++it;
				}
				// create next batch if no place available
				if (it == last)
				{
					it = m_batches.emplace(it);
				}

				++m_count;

				return{ it, (it->recycled() > 0) ? it->recycle() : it->increment() };
			}

			void destroy(key k)
			{
				std::lock_guard<std::mutex> lock(m_mutex);

				k.batch->recycle(k.cursor);

				--m_count;
			}
		};
	}
}

#endif