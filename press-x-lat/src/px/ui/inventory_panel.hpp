// name: inventory_panel.hpp
// type: c++
// desc: base class declaration
// auth: is0urce

#ifndef PX_UI_INVENTORY_PANEL_HPP
#define PX_UI_INVENTORY_PANEL_HPP

#include <px/ui/stack_panel.hpp>

#include <px/ui/board_panel.hpp>
#include <px/ui/static_text_panel.hpp>
#include <px/ui/text_panel.hpp>

#include <px/rl/inventory.hpp>
#include <px/rl/effect.hpp>

#include <string>
#include <memory>

namespace px
{
	namespace ui
	{
		class inventory_panel : public stack_panel
		{
		public:
			typedef rl::inventory<rl::effect> inventory_type;
			typedef std::weak_ptr<inventory_type> inventory_ptr;

		private:
			inventory_ptr m_inventory;
		public:
			inventory_panel()
			{
				emplace<board_panel>({ { 0, 0 }, { 0, 0 }, { 0, 1 }, { 1, 0 } }, color(0, 0, 1));
				emplace<board_panel>({ { 0, 0 }, { 0, 1 }, { 0, -1 }, { 1, 1 } }, color(0, 0, 0.5));
				emplace<static_text_panel>({ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, "[INVENTORY]", color(1, 1, 1));
			}
			virtual ~inventory_panel() {}

		protected:
			virtual void draw_panel(shell::canvas& cnv) const override
			{
				stack_panel::draw_panel(cnv);

				auto inventory = m_inventory.lock();
				if (inventory)
				{
					auto pen = bounds().start();
					pen.move_axis<1>(1);

					inventory->enumerate([&](const auto &item) {
						cnv.write(pen, item->name());
						pen.move_axis<1>(1);
					});
				}
			}

		public:
			void show(inventory_ptr inventory)
			{
				m_inventory = inventory;
			}
		};
	}
}

#endif