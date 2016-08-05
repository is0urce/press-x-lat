// name: anvil_panel.hpp
// type: c++
// desc: base class declaration
// auth: is0urce

#ifndef PX_UI_ANVIL_PANEL_HPP
#define PX_UI_ANVIL_PANEL_HPP

#include <px/ui/stack_panel.hpp>
#include <px/rl/inventory.hpp>
#include <px/rl/effect.hpp>

#include <string>
#include <memory>

namespace px
{
	namespace ui
	{
		class anvil_panel : public stack_panel
		{
		public:
			typedef rl::inventory<rl::effect> inventory_type;
			typedef std::weak_ptr<inventory_type> inventory_ptr;

		private:
			inventory_ptr m_inventory;
		public:
			anvil_panel() {}
			virtual ~anvil_panel() {}

		protected:
			virtual void draw_panel(shell::canvas& cnv) const override
			{
				stack_panel::draw_panel(cnv);

				auto pen = bounds().start();

				auto inventory = m_inventory.lock();
				if (inventory)
				{
					cnv.write(pen, "[CRAFTING]");
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