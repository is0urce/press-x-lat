// name: container_panel.hpp
// type: c++
// desc: base class declaration
// auth: is0urce

#ifndef PX_CORE_UI_CONTAINER_PANEL_HPP
#define PX_CORE_UI_CONTAINER_PANEL_HPP

#include <px/ui/stack_panel.hpp>

#include <px/ui/board.hpp>
#include <px/ui/text.hpp>
#include <px/ui/button.hpp>
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
			struct take_all_operator;
			struct move_item_operator;

			typedef rl::inventory<rl::effect> inventory_type;
			typedef ui::list_panel<inventory_type, item_formatter, item_color, move_item_operator> list_type;
			typedef std::weak_ptr<inventory_type> inventory_observer;

			struct move_item_operator
			{
				template <typename I>
				bool operator()(I item)
				{
					if (from && to)
					{
						auto source = from->lock();
						auto destination = to->lock();
						if (source && destination)
						{
							source->remove(item);
							destination->add(item);
						}
					}
					return true;
				};

				move_item_operator() : from(nullptr), to(nullptr) {}
				move_item_operator(inventory_observer* source, inventory_observer* dest) : from(source), to(dest) {}

				inventory_observer* from;
				inventory_observer* to;
			};

		public:
			void examine_container(inventory_observer user, inventory_observer container)
			{
				m_user = user;
				m_container = container;
				m_user_list->bind_data(user);
				m_container_list->bind_data(container);

				update();
			}

		public:
			container_panel()
			{
				emplace<ui::board>({ { 0.0, 0.0 },{ 0, 0 },{ 0, 1 },{ 1.0, 0.0 } }, color(0, 0, 1));
				emplace<ui::board>({ { 0.0, 0.0 },{ 0, 1 },{ 0, -1 },{ 1.0, 1.0 } }, color(0, 0, 0.5));
				emplace<ui::text>({ { 0.0, 0.0 },{ 0, 0 },{ 0, 1 },{ 0.5, 0.0 } }, "[YOU]", color(1, 1, 1));
				emplace<ui::text>({ { 0.5, 0.0 },{ 0, 0 },{ 0, 1 },{ 0.5, 0.0 } }, "[NOT YOU]", color(1, 1, 1));

				m_user_list = emplace<list_type>({ { 0.0, 0.0 }, { 0, 1 }, { 0, -2 }, { 0.5, 1.0 } }).get();
				m_container_list = emplace<list_type>({ { 0.5, 0.0 },{ 0, 1 },{ 0, -2 },{ 0.5, 1.0 } }).get();

				m_take_all = emplace<ui::button>("take_all", { { 1.0, 1.0 },{ -10, -1 },{ 10, 1 },{ 0.0, 0.0 } }, color::black(), color::black(), "take all", color::white(), [this](auto const&, auto) { take_all(); return true; }).get();

				m_user_list->set_click(move_item_operator(&m_user, &m_container));
				m_container_list->set_click(move_item_operator(&m_container, &m_user));
			}
			virtual ~container_panel() {}

		protected:
			virtual bool click_stacked(const point2 &position, unsigned int button) override
			{
				bool close = !bounds().contains(position);
				if (close)
				{
					deactivate();
				}

				update();

				return close;
			}
			virtual bool key_stacked(shell::key code) override
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
				auto loot = m_container.lock();
				m_take_all->set_toggle(loot && !loot->empty());
			}
			void take_all()
			{
				auto user = m_user.lock();
				auto loot = m_container.lock();
				if (user && loot)
				{
					user->take_all(*loot);
				}
			}

		private:
			inventory_observer m_user;
			inventory_observer m_container;
			list_type* m_user_list;
			list_type* m_container_list;
			ui::button* m_take_all;
		};
	}
}

#endif