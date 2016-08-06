// name: container_panel.hpp
// type: c++
// desc: base class declaration
// auth: is0urce

#ifndef PX_CORE_UI_CONTAINER_PANEL_HPP
#define PX_CORE_UI_CONTAINER_PANEL_HPP

#include <px/ui/stack_panel.hpp>

#include <px/ui/board_panel.hpp>
#include <px/ui/static_text_panel.hpp>
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
		class container_panel : public ui::stack_panel
		{
		public:
			typedef rl::inventory<rl::effect> inventory_type;
			typedef ui::list_panel<inventory_type, item_formatter, item_color, item_filter> list_type;
			typedef std::weak_ptr<inventory_type> inventory_ptr;

		private:
			std::shared_ptr<list_type> m_user;
			std::shared_ptr<list_type> m_container;

		public:
			container_panel()
			{
				emplace<ui::board_panel>({ { 0, 0 },{ 0, 0 },{ 0, 1 },{ 1, 0 } }, color(0, 0, 1));
				emplace<ui::board_panel>({ { 0, 0 },{ 0, 1 },{ 0, -1 },{ 1, 1 } }, color(0, 0, 0.5));
				emplace<ui::static_text_panel>({ { 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 } }, "[YOU]", color(1, 1, 1));
				emplace<ui::static_text_panel>({ { 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 } }, "[NOT YOU]", color(1, 1, 1));

				m_user = std::make_shared<list_type>();
				add(m_user, { { 0.0, 0 },{ 0, 1 },{ 0, -1 },{ 0.5, 1 } });

				m_container = std::make_shared<list_type>();
				add(m_container, { { 0.5, 0 },{ 0, 1 },{ 0, -1 },{ 0.5, 1 } });
			}
			virtual ~container_panel() {}

		protected:
			virtual void draw_panel(shell::canvas& cnv) const override
			{
				stack_panel::draw_panel(cnv);
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
			virtual bool key_control(shell::key code) override
			{
				bool close = code == shell::key::command_cancel || code == shell::key::move_none;
				if (close)
				{
					deactivate();
				}
				return close;
			}

		public:
			void examine_container(inventory_ptr user, inventory_ptr container)
			{
				m_user->bind_data(user);
				m_container->bind_data(container);
			}
		};
	}
}

#endif