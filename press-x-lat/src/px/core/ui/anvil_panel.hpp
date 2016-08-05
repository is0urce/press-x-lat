// name: anvil_panel.hpp
// type: c++
// desc: base class declaration
// auth: is0urce

#ifndef PX_UI_ANVIL_PANEL_HPP
#define PX_UI_ANVIL_PANEL_HPP

#include <px/ui/stack_panel.hpp>

#include <px/ui/board_panel.hpp>
#include <px/ui/static_text_panel.hpp>

#include <px/rl/inventory.hpp>
#include <px/rl/effect.hpp>

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
			typedef std::weak_ptr<inventory_type> inventory_ptr;

		private:
			inventory_ptr m_inventory;
		public:
			anvil_panel()
			{
				emplace<ui::board_panel>({ { 0, 0 },{ 0, 0 },{ 0, 1 },{ 1, 0 } }, color(0, 0, 1));
				emplace<ui::board_panel>({ { 0, 0 },{ 0, 1 },{ 0, -1 },{ 1, 1 } }, color(0, 0, 0.5));
				emplace<ui::static_text_panel>({ { 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 } }, "[K IS FOR KRAFT]", color(1, 1, 1));
			}
			virtual ~anvil_panel() {}

		protected:
			virtual void draw_panel(shell::canvas& cnv) const override
			{
				stack_panel::draw_panel(cnv);

				auto pen = start();

				auto inventory = m_inventory.lock();
				if (inventory)
				{
					pen.move_axis<1>(1);

					cnv.write(pen, "Sword");
					pen.move_axis<1>(1);
					cnv.write(pen, "Bow");
					pen.move_axis<1>(1);
					cnv.write(pen, "Helmet");
					pen.move_axis<1>(1);

					inventory->enumerate([&](const auto &item) {
						cnv.write(pen, item->name());
						pen.move_axis<1>(1);
					});
				}
			}
			virtual bool click_control(const point2 &position, unsigned int button) override
			{
				return false;
			}

		public:
			void bind(inventory_ptr inventory)
			{
				m_inventory = inventory;
			}
		};
	}
}

#endif