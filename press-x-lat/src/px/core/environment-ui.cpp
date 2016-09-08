// name: environment-ui.cpp
// type: c++ header
// desc: class methods implementation
// auth: is0urce

#include "environment.hpp"

#include <px/common/fps_counter.hpp>

#include <px/ui/stack_panel.hpp>
#include <px/ui/performance_panel.hpp>
#include <px/ui/button.hpp>
#include <px/ui/progress_bar.hpp>

#include <px/core/ui/title_panel.hpp>
#include <px/core/ui/ingame_panel.hpp>

#include <px/core/ui/status_panel.hpp>
#include <px/core/ui/target_panel.hpp>
#include <px/core/ui/inventory_panel.hpp>
#include <px/core/ui/container_panel.hpp>
#include <px/core/ui/anvil_panel.hpp>
#include <px/core/ui/map_panel.hpp>

#include <px/core/sys/body_component.hpp>

namespace px
{
	namespace core
	{
		void environment::compose_ui()
		{
			// fps counter
			auto fps = m_ui->emplace<ui::performance_panel>("performance", { { 0.0, 0.0 },{ 1,0 },{ -2, 1 },{ 1.0, 0.0 } }, m_fps);
			auto title = m_ui->emplace<title_panel>("title", ui::alignment::fill());
			{
				auto menu = title->emplace<ui::stack_panel>("menu", ui::alignment::fill());
				auto create = title->emplace<ui::stack_panel>("create", ui::alignment::fill());

				auto exit = [this](auto const& location, unsigned int vk) { deactivate(); return true; };
				auto start = [menu = menu.get(), create = create.get()](auto const& location, unsigned int vk) { menu->deactivate(); create->activate(); return true; };

				// menu
				{
					auto label = menu->emplace<ui::text>({ { 0.0, 0.0 },{ 0, 1 },{ 0, 0 },{ 1.0, 1.0 } }, "Light-a-Torch", color::white());
					label->set_alignment(ui::text_alignment::center);

					auto start_button = menu->emplace<ui::button>("create", { {0.5, 0.5}, {-8, 0}, {15, 1}, {0.0, 0.0} }, 0x000000, 0x333333, "create", color::white(), start);
					auto exit_button = menu->emplace<ui::button>("exit", { { 0.5, 0.5 },{ -8, 2 },{ 15, 1 },{ 0.0, 0.0 } }, 0x000000, 0x333333, "exit", color::white(), exit);

					start_button->text()->set_alignment(ui::text_alignment::center);
					exit_button->text()->set_alignment(ui::text_alignment::center);
				}

				// create
				{
					auto label = create->emplace<ui::text>({ { 0.0, 0.0 },{ 0, 1 },{ 0, 0 },{ 1.0, 1.0 } }, "Create your world...", color::white());
					label->set_alignment(ui::text_alignment::center);

					std::vector<std::pair<world_aspect, std::string>> aspects{
						{ world_aspect::inertia,  "Inertia:" },
						{ world_aspect::knowledge,  "Knowledge:" },
						{ world_aspect::sentience,  "Sentience:" },
						{ world_aspect::entropy,  "Entropy:" },
						{ world_aspect::existence,  "Existence:" } };

					int i = 0;
					for (auto& aspect : aspects)
					{
						create->emplace<ui::progress_bar>({ {0.5, 0.5}, {-5, i * 2}, {10, 1}, {0.0, 0.0} },
							0xcccccc, 0x999999,
							[aspect = std::get<0>(aspect), this]() { return std::pair<int, int>(m_settings[aspect], world_settings::max_value); });
						create->emplace<ui::button>({ { 0.5, 0.5 },{ -12, i * 2 },{ 1, 1 },{ 0.0, 0.0 } },
							color::black(), 0x333333, "-", color::white(),
							[aspect = std::get<0>(aspect), this](auto const&, auto) { m_settings.decrement(aspect); return true; });
						create->emplace<ui::button>({ { 0.5, 0.5 },{ 12, i * 2 },{ 1, 1 },{ 0.0, 0.0 } },
							color::black(), 0x333333, "+", color::white(),
							[aspect = std::get<0>(aspect), this](auto const&, auto) { m_settings.increment(aspect); return true; });
						create->emplace<ui::text>({ { 0.5, 0.5 },{ -30, i * 2 },{ 1, 1 },{ 0.0, 0.0 } }, std::get<1>(aspect), color::white());

						++i;
					}
				}

				menu->activate();
				create->deactivate();
			}
			auto ingame = m_ui->emplace<ingame_panel>("ingame", ui::alignment::fill());
			{
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
			}

			title->activate();
			ingame->deactivate();
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
		void environment::tie_map()
		{
			m_map->bind(m_world);
		}
		void environment::untie_map()
		{
			m_map->tear();
		}
	}
}