// name: inventory.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_RL_INVENTORY_HPP
#define PX_RL_INVENTORY_HPP

#include <px/rl/item.hpp>

#include <list>
#include <memory>

namespace px
{
	namespace rl
	{
		class inventory
		{
		public:
			typedef std::shared_ptr<item> item_ptr;
		private:
			std::list<item_ptr> m_items;

		public:
			void add(item_ptr i)
			{
				m_items.push_back(i);
			}
			void remove(const item_ptr &i)
			{
				m_items.remove(i);
			}
			void clear()
			{
				m_items.clear();
			}
		};
	}
}

#endif