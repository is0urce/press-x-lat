// name: inventory_panel.hpp
// type: c++
// desc: base class declaration
// auth: is0urce

#ifndef PX_UI_INVENTORY_PANEL_HPP
#define PX_UI_INVENTORY_PANEL_HPP

#include <px/ui/stack_panel.hpp>
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
			inventory_panel() {}
			virtual ~inventory_panel() {}

		protected:
			virtual void draw_panel(shell::canvas& cnv) const override
			{
				auto pen = bounds().start();

				auto inventory = m_inventory.lock();
				if (inventory)
				{
					cnv.write(pen, "[INVENTORY]");
					pen.move_axis<1>(1);

					inventory->enumerate([&](const auto &item) {
						cnv.write(pen, item->name());
					});
				}
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