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
		template<typename E, size_t _B>
		class manager
		{
		public:
			typedef E element;
			typedef std::recursive_mutex mutex_type;
			struct element_record
			{
				bool exists;
				E entry;
				element_record() : exists(false) {}
			};
			struct batch
			{
			private:
				std::array<element_record, _B> m_elements;
				std::list<size_t> m_recycle;
				size_t m_cursor;

			public:
				batch() : m_cursor(0)
				{
				}

			public:
				bool exists(size_t n) const
				{
					return m_elements[n].exists;
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
					for (auto it = m_elements.begin(), last = m_elements.end(); it != last; ++it)
					{
						it->exists = false;
					}
				}
				void optimise()
				{
					if (count() == 0)
					{
						clear();
					}
				}
				const element& operator[](size_t index) const
				{
					return m_elements[index].entry;
				}
				element& operator[](size_t index)
				{
					return m_elements[index].entry;
				}
				const element& select(size_t index) const
				{
					return m_elements[index].entry;
				}
				element& select(size_t index)
				{
					return m_elements[index].entry;
				}
				size_t increment()
				{
					size_t index = m_cursor;
					++m_cursor;

					m_elements[index].exists = true;
					return index;
				}
				void recycle(size_t index)
				{
					m_recycle.push_back(index);
					m_elements[index].exists = false;

					// optimise batch
					optimise();
				}
				size_t recycle()
				{
					// use position of recycled component
					auto it = m_recycle.rbegin();
					auto index = *it;
					m_recycle.pop_back();
					m_elements[index].exists = true;
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
			mutex_type m_mutex;
			size_t m_count = 0;

		public:
			virtual ~manager() {}
			manager() {}
			manager(manager const&) = delete;
			manager& operator=(manager const&) = delete;

		protected:
			virtual void element_allocated(element &) {}
			virtual void element_released(element &) {}

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

				return std::shared_ptr<element>(&e, [this, k](element* pointer) {

					destroy(k);
					if (pointer)
					{
						element_released(*pointer);
					}
				});
			}

			template<typename Op>
			void enumerate(Op&& fn)
			{
				std::lock_guard<mutex_type> lock(m_mutex);

				for (auto it = m_batches.begin(), last = m_batches.end(); it != last; ++it)
				{
					size_t cursor = it->cursor();
					for (size_t i = 0; i < cursor; ++i)
					{
						if (it->exists(i))
						{
							std::forward<Op>(fn)((*it)[i]);
						}
					}
				}
			}

			// clear unused batches
			void optimise()
			{
				std::lock_guard<mutex_type> lock(m_mutex);

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
				std::lock_guard<mutex_type> lock(m_mutex);

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

				size_t cursor;

				if (it->recycled() > 0)
				{
					cursor = it->recycle();
				}
				else
				{
					cursor = it->increment();
				}

				key result{ it, cursor };

				++m_count;
				return result;
			}

			void destroy(key k)
			{
				std::lock_guard<mutex_type> lock(m_mutex);

				k.batch->recycle(k.cursor);

				--m_count;
			}
		};
	}
}

#endif