// name: inventory_panel.hpp
// type: c++
// desc: base class declaration
// auth: is0urce

#ifndef PX_UI_INVENTORY_PANEL_HPP
#define PX_UI_INVENTORY_PANEL_HPP

#include <px/ui/stack_panel.hpp>

#include <px/ui/board_panel.hpp>
#include <px/ui/static_text_panel.hpp>
#include <px/ui/list_panel.hpp>

#include <px/rl/inventory.hpp>
#include <px/rl/effect.hpp>

#include <px/shell/key.hpp>

#include <string>
#include <memory>

namespace px
{
	namespace core
	{
		class inventory_panel : public ui::stack_panel
		{
		public:
			static const unsigned int line_size = 1;
		public:
			struct name_formatter
			{
				template <typename I>
				std::string operator()(const I& item) const
				{
					return item->name();
				}
			};
			typedef rl::inventory<rl::effect> inventory_type;
			typedef std::weak_ptr<inventory_type> inventory_ptr;
			typedef ui::list_panel<inventory_type, name_formatter> list_type;

		private:
			inventory_ptr m_inventory;
			std::shared_ptr<list_type> m_list;

		public:
			inventory_panel()
			{
				emplace<ui::board_panel>({ { 0, 0 }, { 0, 0 }, { 0, 1 }, { 1, 0 } }, color(0, 0, 1));
				emplace<ui::board_panel>({ { 0, 0 }, { 0, 1 }, { 0, -1 }, { 1, 1 } }, color(0, 0, 0.5));
				emplace<ui::static_text_panel>({ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, "[INVENTORY]", color(1, 1, 1));

				m_list = std::make_shared<list_type>();
				add("list", m_list, { { 0, 0 },{ 0, 1 },{ 0, -1 },{ 1, 1 } });
			}
			virtual ~inventory_panel() {}

		protected:
			virtual void draw_panel(shell::canvas& cnv) const override
			{
				stack_panel::draw_panel(cnv);
			}
			virtual bool key_control(shell::key code) override
			{
				bool result = stack_panel::key_control(code);

				if (!result)
				{
					bool result = code == shell::key::command_cancel || code == shell::key::move_none;
					if (result)
					{
						deactivate();
					}
				}
				return result;
			}

		public:
			void show(inventory_ptr inventory)
			{
				m_list->bind_data(inventory);
			}
		};
	}
}

#endif