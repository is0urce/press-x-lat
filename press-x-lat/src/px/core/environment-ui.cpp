// name: environment-ui.cpp
// type: c++ header
// desc: class methods implementation
// auth: is0urce

#include "environment.hpp"

#include <px/common/fps_counter.hpp>

#include <px/ui/stack_panel.hpp>

#include <px/core/ui/status_panel.hpp>
#include <px/core/ui/target_panel.hpp>
#include <px/core/ui/inventory_panel.hpp>
#include <px/core/ui/container_panel.hpp>
#include <px/core/ui/anvil_panel.hpp>
#include <px/core/ui/map_panel.hpp>
#include <px/core/ui/title_panel.hpp>
#include <px/core/ui/ingame_panel.hpp>

#include <px/ui/performance_panel.hpp>

#include <px/core/sys/body_component.hpp>

namespace px
{
	namespace core
	{
		void environment::compose_ui()
		{
			auto title = m_ui->emplace<title_panel>("title", ui::alignment::fill());
			auto ingame = m_ui->emplace<ingame_panel>("ingame", ui::alignment::fill());

			auto status = ingame->emplace<status_panel>("status", { { 0.0, 0.0 },{ 1, 2 },{ -2, 1 },{ 1.0, 0.0 } }, *this);
			auto target = ingame->emplace<target_panel>("target", { { 1.0, 0.0 },{ -12, 2 },{ -2, 1 },{ 1.0, 0.0 } }, *this);
			m_inventory = ingame->emplace<inventory_panel>("inventory", { { 0.3, 0.1 },{ 0, 0 },{ 0, 0 },{ 0.4, 0.8 } });
			m_craft = ingame->emplace<anvil_panel>("craft", { { 0.1, 0.1 },{ 0, 0 },{ 0, 0 },{ 0.8, 0.8 } });
			m_container = ingame->emplace<container_panel>("container", { { 0.2, 0.1 },{ 0, 0 },{ 0, 0 },{ 0.6, 0.8 } });
			m_map = ingame->emplace<map_panel>("map", { { 0.1, 0.1 },{ 0, 0 },{ 0, 0 },{ 0.8, 0.8 } });

			// start with ingame panels closed
			m_inventory->deactivate();
			m_craft->deactivate();
			m_container->deactivate();
			m_map->deactivate();

			// fps counter
			m_ui->emplace<ui::performance_panel>("performance", { { 0.0, 0.0 },{ 1,0 },{ -2, 1 },{ 1.0, 0.0 } }, m_fps);
		}

		void environment::target(point2 location)
		{
			m_target = location;
		}
		point2 environment::targeted() const
		{
			return m_target;
		}

		void environment::assign_inventory(std::weak_ptr<body_component> body)
		{
			m_inventory->show(body);
		}
		void environment::open_workshop(std::weak_ptr<body_component> user)
		{
			m_craft->show(user);

			m_inventory->deactivate();
			m_container->deactivate();

			m_craft->activate();
		}
		void environment::open_container(std::weak_ptr<body_component> user, std::weak_ptr<body_component> container)
		{
			m_container->examine_container(user, container);

			m_inventory->deactivate();
			m_craft->deactivate();

			m_container->activate();
		}
	}
}