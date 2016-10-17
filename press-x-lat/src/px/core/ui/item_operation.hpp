// name: item_operation.hpp
// type: c++
// desc: operators
// auth: is0urce

#ifndef PX_CORE_UI_ITEM_OPERATION_HPP
#define PX_CORE_UI_ITEM_OPERATION_HPP

#include <px/common/color.hpp>

#include <px/rl/inventory.hpp>
#include <px/rl/effect.hpp>

namespace px
{
	namespace core
	{
		struct item_formatter
		{
			template <typename I>
			std::string operator()(const I& item) const
			{
				std::string text = item->name();
				unsigned int stack = item->stack_size();
				if (stack != 1) text += std::string(" (") + std::to_string(stack) + std::string(")");
				return text;
			}
		};
		struct item_color
		{
			template <typename I>
			color operator()(I const&) const
			{
				return m_color;
			}
			item_color() : item_color({1, 1, 1})
			{
			}
			item_color(color c) : m_color(c)
			{

			}
		private:
			color m_color;
		};
		struct item_filter
		{
			template <typename I>
			bool operator()(I const& item) const
			{
				return item ? true : false;
			}
		};
	}
}

#endif