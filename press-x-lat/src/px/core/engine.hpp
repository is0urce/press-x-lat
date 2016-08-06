// name: engine.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_CORE_ENGINE_HPP
#define PX_CORE_ENGINE_HPP

#include <px/common/toggle.hpp>

#include <px/es/i_engine.hpp>

#include <px/core/sys/rendering_system.hpp>
#include <px/core/sys/ui_system.hpp>
#include <px/core/sys/location_system.hpp>
#include <px/core/sys/terrain_system.hpp>
#include <px/core/sys/body_system.hpp>
#include <px/core/sys/character_system.hpp>
#include <px/core/sys/behavior_system.hpp>

#include "unit.hpp"
#include "game.hpp"
#include "terrain.hpp"

#include "input_adapter.hpp"

#include <px/data/factory.hpp>
#include <px/rl/skill.hpp>

#include <px/core/ui/main_panel.hpp>

#include <px/shell/control.hpp>
#include <px/shell/control_chain.hpp>
#include <px/shell/opengl.h>
#include <px/shell/renderer.hpp>

namespace px
{
	namespace
	{
		// should be power of 2, size not matters while it grows
		const unsigned int space_start_width = 64;
	}
	namespace core
	{
		class engine
			: public es::i_engine
			, public shell::control_chain
			, public toggle<true>
		{
		private:
			input_adapter m_game_adapter;

			qtree<location_component*> m_space;
			shell::renderer m_renderer;
			shell::canvas m_canvas;
			main_panel m_ui;

			location_system m_location_system;
			rendering_system m_sprite_system;
			terrain_system m_terrain_system;
			body_system m_body_system;
			character_system m_character_system;
			behavior_system m_behavior_system;
			ui_system m_ui_system;

			terrain m_terrain;
			environment m_environment;
			game m_game;
			unsigned int m_last_turn;

			data::factory m_factory;
			world m_world;

		public:
			engine(shell::opengl* gl)
				: control_chain(m_ui, m_game_adapter, [this](point2 pixel) { return translate_canvas(pixel); }, [this](point2 pixel) { return translate_world(pixel); })
				, m_game_adapter(m_game)
				, m_renderer(gl)
				, m_space(space_start_width)
				, m_canvas(1, 1)
				, m_ui_system(m_canvas, m_ui)
				, m_location_system(m_space)
				, m_sprite_system(m_canvas)
				, m_terrain_system(m_canvas, m_terrain)
				, m_behavior_system(m_environment)
				, m_factory(m_sprite_system, m_location_system, m_body_system, m_character_system, m_behavior_system)
				, m_world(m_factory)
				, m_terrain(m_world)
				, m_environment(m_ui, m_terrain, m_space)
				, m_game(m_environment)
				, m_last_turn(0)
			{
				add_systems();

				m_character_system.skill_book().add_target("melee", [](location_component* user, location_component* target) {
					if (target && user)
					{
						body_component* target_body = *target;
						body_component* user_body = *user;
						if (target_body && user_body)
						{
							if (auto hp = target_body->health())
							{
								auto dmg = user_body->weapon()->accumulate<rl::effect::weapon_damage>();
								hp->damage(dmg.value0);
							}
						}
					}
				}, [this](location_component* user, location_component* target) {
					if (!user || !target) return false;

					body_component* body = *user;
					body_component* target_body = *target;

					return body && target_body && body != target_body
						&& target_body->health() && !target_body->health()->empty() // should have positive hit points
						&& m_environment.reputation(*body, *target_body) < 0 // friend-or-foe
						&& m_environment.distance(user->current(), target->current()) == 1; // 1 tile melee distance
				});


				// player props
				auto weapon = std::make_shared<body_component::item_type>();
				weapon->add({ rl::effect::weapon_damage, 0x00, 1 });
				weapon->set_name("Copper Sword");
				weapon->set_tag("copper_sword");
				auto ore = std::make_shared<body_component::item_type>();
				ore->add({ rl::effect::ore_power, 0x00, 10 });
				ore->set_name("Ore");
				ore->set_tag("common_ore");
				ore->set_description("This is a lump of ore, can be mined from ore vein deposits or bought from merchants.");
				auto vein = std::make_shared<core::resource_component>();
				vein->deposit(ore);

				// make chest
				auto chest = m_factory.produce();
				chest->add_appearance('$', { 1, 1, 1 });
				chest->add_location({ 2, 2 });
				auto cb = chest->add_body(100, 100);
				auto container = std::make_shared<core::container_component>();
				chest->add(container);
				// setup
				cb->add(weapon);
				cb->add(weapon);
				cb->add(weapon);
				// add
				m_terrain.add(chest->assemble());

				// make player
				auto task = m_factory.produce();
				task->add_appearance('@', { 1, 1, 1 });
				auto pawn = task->add_location({ 1, 1 });
				auto body = task->add_body(100, 100);
				auto character = task->add_character();
				task->add(vein);
				// setup
				body->join_faction(1);
				body->equip_weapon(weapon);
				for (int i = 0; i < 25; ++i)
				{
					auto item = std::make_shared<body_component::item_type>();
					item->set_name("Amulet of Abstract Number " + std::to_string(i));
					body->add(item);
				}
				character->add_skill("melee");
				// add
				m_terrain.add(task->assemble());

				m_environment.impersonate(pawn);
			}
			virtual ~engine()
			{
			}
			engine(const engine&) = delete;

		private:
			point2 translate_canvas(point2 pixel) const
			{
				return m_renderer.translate_canvas(pixel);
			}
			point2 translate_world(point2 pixel) const
			{
				auto pos = translate_canvas(pixel) - (m_canvas.range() / 2);
				pos.mirror<1>(); // flip horisontally
				if (auto camera = m_environment.player())
				{
					pos += camera->current();
				}
				return pos;
			}
			void add_systems()
			{
				// mechanics systems

				add(&m_body_system);
				add(&m_character_system);
				add(&m_location_system);
				add(&m_behavior_system);

				// visualization systems

				add(&m_terrain_system);
				add(&m_sprite_system);
				add(&m_ui_system);
			}

		protected:
			virtual void pre_update_engine() override
			{
				int w, h;
				m_renderer.canvas_size(w, h);
				if (w > 0 && h > 0)
				{
					m_canvas.resize(w, h);
					m_ui.layout(m_canvas.range());
				}
				m_canvas.cls();

				auto focus = m_environment.player();

				m_sprite_system.focus_camera(focus);
				m_terrain_system.focus_camera(focus);
			}
			virtual void post_update_engine() override
			{
				m_renderer.render(0, m_canvas);
			}
			virtual bool require_fixed() override
			{
				bool result = false;
				auto turn = m_environment.time();
				if (m_last_turn != turn)
				{
					m_last_turn = turn;
					result = true;
				}
				return result;
			}
		};
	}
}

#endif