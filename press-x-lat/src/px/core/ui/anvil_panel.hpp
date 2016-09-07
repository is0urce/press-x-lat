// name: anvil_panel.hpp
// type: c++
// desc: base class declaration
// auth: is0urce

#ifndef PX_UI_ANVIL_PANEL_HPP
#define PX_UI_ANVIL_PANEL_HPP

#include <px/ui/stack_panel.hpp>

#include <px/ui/board.hpp>
#include <px/ui/text.hpp>
#include <px/ui/list_panel.hpp>

#include <px/rl/inventory.hpp>
#include <px/rl/effect.hpp>

#include <px/core/ui/item_operation.hpp>

#include <px/shell/key.hpp>

#include <string>
#include <memory>

namespace px
{
	namespace core
	{
		class anvil_panel : public ui::stack_panel
		{
		public:
			typedef rl::inventory<rl::effect> inventory_type;
			typedef ui::list_panel<inventory_type, item_formatter, item_color, item_filter> list_type;
			typedef std::weak_ptr<inventory_type> inventory_ptr;

		private:
			std::shared_ptr<list_type> m_list;

		public:
			anvil_panel()
			{
				emplace<ui::board>({ { 0, 0 },{ 0, 0 },{ 0, 1 },{ 1, 0 } }, color(0, 0, 1));
				emplace<ui::board>({ { 0, 0 },{ 0, 1 },{ 0, -1 },{ 1, 1 } }, color(0, 0, 0.5));
				emplace<ui::text>({ { 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 } }, "[K IS FOR KRAFT]", color(1, 1, 1));

				m_list = std::make_shared<list_type>();
				add(m_list, { { 0.3, 0 },{ 0, 1 },{ 0, -1 },{ 0.3, 1 } });
			}
			virtual ~anvil_panel() {}

		protected:
			virtual void draw_panel(shell::canvas& cnv) const override
			{
				stack_panel::draw_panel(cnv);

				auto pen = start();

				pen.move_axis<1>(1);

				cnv.write(pen, "Sword");
				pen.move_axis<1>(1);
				cnv.write(pen, "Bow");
				pen.move_axis<1>(1);
				cnv.write(pen, "Helmet");
				pen.move_axis<1>(1);
			}
			virtual bool key_control(shell::key code) override
			{
				bool close = code == shell::key::command_cancel || code == shell::key::move_none;
				if (close)
				{
					deactivate();
				}
				return close;
			}
			virtual bool click_control(const point2 &position, unsigned int button) override
			{
				bool result = stack_panel::click_control(position, button);
				if (!result && !bounds().contains(position))
				{
					deactivate();
					result = true;
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