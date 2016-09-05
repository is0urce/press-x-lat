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
#include <px/ui/press_panel.hpp>

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
			struct take_all_operator;
			struct move_item_operator;

			typedef rl::inventory<rl::effect> inventory_type;
			typedef ui::list_panel<inventory_type, item_formatter, item_color, move_item_operator> list_type;
			typedef std::weak_ptr<inventory_type> inventory_observer;

			struct take_all_operator
			{
				bool operator()(unsigned int v_button)
				{
					panel->take_all();
					return true;
				};

				take_all_operator(container_panel* cp) : panel(cp) {}

				container_panel* panel;
			};
			struct move_item_operator
			{
				template <typename I>
				bool operator()(I item)
				{
					if (from && to)
					{
						auto s = from->lock();
						auto d = to->lock();
						if (s && d)
						{
							s->remove(item);
							d->add(item);
						}
					}
					return true;
				};

				move_item_operator() : from(nullptr), to(nullptr) {}
				move_item_operator(inventory_observer* source, inventory_observer* dest) : from(source), to(dest) {}

				inventory_observer* from;
				inventory_observer* to;
			};
		private:
			inventory_observer m_user;
			inventory_observer m_container;
			std::shared_ptr<list_type> m_user_list;
			std::shared_ptr<list_type> m_container_list;

		public:
			container_panel()
			{
				emplace<ui::board_panel>({ { 0.0, 0.0 },{ 0, 0 },{ 0, 1 },{ 1.0, 0.0 } }, color(0, 0, 1));
				emplace<ui::board_panel>({ { 0.0, 0.0 },{ 0, 1 },{ 0, -1 },{ 1.0, 1.0 } }, color(0, 0, 0.5));
				emplace<ui::static_text_panel>({ { 0.0, 0.0 },{ 0, 0 },{ 0, 1 },{ 0.5, 0.0 } }, "[YOU]", color(1, 1, 1));
				emplace<ui::static_text_panel>({ { 0.5, 0.0 },{ 0, 0 },{ 0, 1 },{ 0.5, 0.0 } }, "[NOT YOU]", color(1, 1, 1));

				m_user_list = std::make_shared<list_type>();
				add(m_user_list, { { 0.0, 0.0 },{ 0, 1 },{ 0, -2 },{ 0.5, 1.0 } });

				m_container_list = std::make_shared<list_type>();
				add(m_container_list, { { 0.5, 0.0 },{ 0, 1 },{ 0, -2 },{ 0.5, 1.0 } });

				emplace<ui::static_text_panel>("take_all", { { 1.0, 1.0 },{ -10, -1 },{ 10, 1 },{ 0.0, 0.0 } }, "[take all]", color(1, 1, 1));
				emplace<ui::press_panel<take_all_operator>>({ { 1.0, 1.0 },{ -10, -1 },{ 10, 1 },{ 0.0, 0.0 } }, this);

				m_user_list->set_click(move_item_operator(&m_user, &m_container));
				m_container_list->set_click(move_item_operator(&m_container, &m_user));
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

				update();

				return result;
			}
			virtual bool key_control(shell::key code) override
			{
				bool close = code == shell::key::command_cancel || code == shell::key::move_none;
				if (close)
				{
					deactivate();
				}

				update();

				return close;
			}

		private:
			void update()
			{
				if (auto cont = m_container.lock())
				{
					activate("take_all", !cont->empty());
				}
			}
			void take_all()
			{
				auto inv = m_user.lock();
				auto cont = m_container.lock();
				if (inv && cont)
				{
					inv->take_all(*cont);
				}
			}

		public:
			void examine_container(inventory_observer user, inventory_observer container)
			{
				m_user = user;
				m_container = container;
				m_user_list->bind_data(user);
				m_container_list->bind_data(container);
			}
		};
	}
}

#endif