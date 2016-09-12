// name: inventory_panel.hpp
// type: c++
// desc: base class declaration
// auth: is0urce

#ifndef PX_UI_INVENTORY_PANEL_HPP
#define PX_UI_INVENTORY_PANEL_HPP

#include <px/ui/stack_panel.hpp>

#include <px/ui/board.hpp>
#include <px/ui/text.hpp>
#include <px/ui/list_panel.hpp>

#include <px/rl/inventory.hpp>
#include <px/rl/effect.hpp>

#include <px/core/ui/item_operation.hpp>

#include <px/shell/key.hpp>

#include <memory>

namespace px
{
	namespace core
	{
		class inventory_panel : public ui::stack_panel
		{
		public:
			typedef rl::inventory<rl::effect> inventory_type;
			typedef ui::list_panel<inventory_type, item_formatter, item_color, item_filter> list_type;
			typedef std::weak_ptr<inventory_type> inventory_ptr;

		public:
			void show(inventory_ptr inventory)
			{
				m_list->bind_data(inventory);
			}

		public:
			inventory_panel()
			{
				emplace<ui::board>({ { 0, 0 }, { 0, 0 }, { 0, 1 }, { 1, 0 } }, color(0, 0, 1));
				emplace<ui::board>({ { 0, 0 }, { 0, 1 }, { 0, -1 }, { 1, 1 } }, color(0, 0, 0.5));
				emplace<ui::text>({ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, "[INVENTORY]", color(1, 1, 1));

				m_list = emplace<list_type>({ { 0, 0 },{ 0, 1 },{ 0, -1 },{ 1, 1 } }).get();
			}
			virtual ~inventory_panel() {}

		protected:
			virtual bool key_stacked(shell::key code) override
			{
				bool close = code == shell::key::command_cancel || code == shell::key::move_none;
				if (close)
				{
					deactivate();
				}
				return close;
			}
			virtual bool click_stacked(const point2 &position, unsigned int) override
			{
				bool close = !bounds().contains(position);
				if (close)
				{
					deactivate();
				}
				return close;
			}

		private:
			list_type* m_list;
		};
	}
}

#endif