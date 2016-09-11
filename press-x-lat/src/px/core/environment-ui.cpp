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

#include <px/core/ui/ingame_panel.hpp>

#include <px/core/ui/status_panel.hpp>
#include <px/core/ui/target_panel.hpp>
#include <px/core/ui/inventory_panel.hpp>
#include <px/core/ui/container_panel.hpp>
#include <px/core/ui/anvil_panel.hpp>
#include <px/core/ui/map_panel.hpp>

#include <px/core/sys/body_component.hpp>

#include <tuple>

namespace px
{
	namespace core
	{
		void environment::compose_ui()
		{
			// fps counter
			auto fps = m_ui->emplace<ui::performance_panel>({ { 0.0, 0.0 },{ 1,0 },{ -2, 1 },{ 1.0, 0.0 } }, m_fps);
			auto title = m_ui->emplace<ui::stack_panel>("title", ui::alignment::fill()).get();
			auto ingame = m_ui->emplace<ingame_panel>("ingame", ui::alignment::fill()).get();

			// title
			{
				auto menu = title->emplace<ui::stack_panel>("menu", ui::alignment::fill()).get();
				auto create = title->emplace<ui::stack_panel>("create", ui::alignment::fill()).get();

				// menu
				{
					auto label = menu->emplace<ui::text>({ { 0.0, 0.0 },{ 0, 1 },{ 0, 0 },{ 1.0, 1.0 } }, "Light-a-Torch", color::white());
					label->align(ui::text_alignment::center);

					auto start_button = menu->emplace<ui::button>({ { 0.5, 0.5 }, { -8, 0 }, { 15, 1 }, { 0.0, 0.0 } },
						color::black(), 0x333333, "create", color::white(),
						[menu, create](auto const& location, auto) { menu->deactivate(); create->activate(); return true; });
					auto exit_button = menu->emplace<ui::button>({ { 0.5, 0.5 }, { -8, 2 }, { 15, 1 }, { 0.0, 0.0 } },
						color::black(), 0x333333, "exit", color::white(),
						[this](auto const& location, auto) { deactivate(); return true; });

					start_button->text()->align(ui::text_alignment::center);
					exit_button->text()->align(ui::text_alignment::center);
				}

				// create
				{
					static const size_t aspect_index = 0;
					static const size_t aspect_name = 1;
					static const size_t aspect_less = 2;
					static const size_t aspect_more = 3;
					std::vector<std::tuple<world_aspect, std::string, std::string, std::string>> aspects;
					aspects.emplace_back(world_aspect::inertia, "Inertia", "Material", "Etheral");
					aspects.emplace_back(world_aspect::knowledge, "Knowledge", "Arcane", "Mastery");
					aspects.emplace_back(world_aspect::sentience, "Sentience", "Will", "Mind");
					aspects.emplace_back(world_aspect::entropy, "Entropy", "Chaos", "Law");
					aspects.emplace_back(world_aspect::existence, "Existence", "Death", "Life");

					auto label = create->emplace<ui::text>({ { 0.0, 0.0 },{ 0, 1 },{ 0, 0 },{ 1.0, 1.0 } }, "Create your world...", color::white());
					label->align(ui::text_alignment::center);

					int i = 0;
					for (auto const& aspect : aspects)
					{
						create->emplace<ui::text>({ { 0.5, 0.5 }, { -30, i * 2 }, { 0, 0 } }, std::get<aspect_name>(aspect) + ":", color::white());
						auto less_text = create->emplace<ui::text>({ { 0.5, 0.5 }, { -9, i * 2 }, { 0, 0 } }, std::get<aspect_less>(aspect), color::white());
						auto more_text = create->emplace<ui::text>({ { 0.5, 0.5 }, { 9, i * 2 }, { 0, 0 } }, std::get<aspect_more>(aspect), color::white());
						less_text->align(ui::text_alignment::right);
						more_text->align(ui::text_alignment::left);

						create->emplace<ui::progress_bar>({ {0.5, 0.5}, {-5, i * 2}, {10, 1} },
							0xcccccc, 0x999999,
							[aspect = std::get<aspect_index>(aspect), this]() { return ui::progress(m_settings[aspect], static_cast<int>(world_settings::max_value)); });

						create->emplace<ui::button>({ { 0.5, 0.5 }, { -7, i * 2 }, { 1, 1 } },
							color::black(), 0x333333, "-", color::white(),
							[aspect = std::get<aspect_index>(aspect), this](auto const&, auto) { m_settings.decrement(aspect); return true; });
						create->emplace<ui::button>({ { 0.5, 0.5 }, { 7, i * 2 }, { 1, 1 } },
							color::black(), 0x333333, "+", color::white(),
							[aspect = std::get<aspect_index>(aspect), this](auto const&, auto) { m_settings.increment(aspect); return true; });

						++i;
					}

					create->emplace<ui::button>({ { 0.5, 1.0 }, { -5, -2 }, { 10, 1 } },
						color::black(), 0x333333, "Incarnate", color::white(),
						[this, title, ingame](auto const&, auto) {	start(); title->deactivate(); ingame->activate(); return true; });
				}

				menu->activate();
				create->deactivate();
			}

			// ingame
			{
				ingame->emplace<status_panel>("status", { { 0.0, 0.0 },{ 1, 2 },{ -2, 1 },{ 1.0, 0.0 } }, *this);
				ingame->emplace<target_panel>("target", { { 1.0, 0.0 },{ -12, 2 },{ -2, 1 },{ 1.0, 0.0 } }, *this);
				m_inventory = ingame->emplace<inventory_panel>("inventory", { { 0.3, 0.1 },{ 0, 0 },{ 0, 0 },{ 0.4, 0.8 } }).get();
				m_craft = ingame->emplace<anvil_panel>("craft", { { 0.1, 0.1 },{ 0, 0 },{ 0, 0 },{ 0.8, 0.8 } }).get();
				m_container = ingame->emplace<container_panel>("container", { { 0.2, 0.1 },{ 0, 0 },{ 0, 0 },{ 0.6, 0.8 } }).get();
				m_map = ingame->emplace<map_panel>("map", { { 0.1, 0.1 },{ 0, 0 },{ 0, 0 },{ 0.8, 0.8 } }).get();

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

		void environment::assign_inventory(std::shared_ptr<body_component> body)
		{
			m_inventory->show(body);
		}
		void environment::open_workshop(std::shared_ptr<body_component> user)
		{
			m_inventory->deactivate();
			m_container->deactivate();

			if (user)
			{
				m_craft->show(user);
				m_craft->activate();
			}
		}
		void environment::open_container(std::shared_ptr<body_component> user, std::shared_ptr<body_component> container)
		{
			m_inventory->deactivate();
			m_craft->deactivate();

			if (user && container)
			{
				m_container->examine(user, container, user->name(), container->name());
				m_container->activate();
			}
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