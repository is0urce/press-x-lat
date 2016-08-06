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
				return item->name();
			}
		};
		struct item_color
		{
			template <typename I>
			color operator()(const I& item) const
			{
				return color(1, 1, 1);
			}
		};
		struct item_filter
		{
			template <typename I>
			bool operator()(const I& item) const
			{
				return item ? true : false;
			}
		};
	}
}

#endif