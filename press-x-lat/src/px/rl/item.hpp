// name: item.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_RL_ITEM_HPP
#define PX_RL_ITEM_HPP

#include <px/rl/enhancement_collection.hpp>

#include <string>

namespace px
{
	namespace rl
	{
		template <typename _E>
		class item : public enhancement_collection<_E>
		{
		private:
			unsigned int m_stack;
			unsigned int m_max_stack;
			std::string m_name;
			std::string m_description;

		public:
			std::string name() const
			{
				return m_name;
			}
			void set_name(std::string name)
			{
				m_name = name;
			}
			bool stack(item& i)
			{
				bool full = false;
				auto total = m_stack + i.m_stack;
				if (total > m_max_stack)
				{
					i.m_stack -= total - m_stack;
					m_stack = m_max_stack;
				}
				else
				{
					i.m_stack = 0;
					m_stack += i.m_stack;
					full = true;
				}
				return full;
			}
		};
	}
}

#endif