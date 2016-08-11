// name: engine.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_CORE_ENGINE_HPP
#define PX_CORE_ENGINE_HPP

#include <px/common/toggle.hpp>

#include <px/es/i_engine.hpp>

#include <px/core/terrain_director.hpp>
#include <px/core/terrain.hpp>
#include <px/core/game.hpp>
#include <px/core/unit.hpp>

#include <px/core/data/factory.hpp>
#include <px/core/ui/main_panel.hpp>

#include <px/core/sys/rendering_system.hpp>
#include <px/core/sys/ui_system.hpp>
#include <px/core/sys/location_system.hpp>
#include <px/core/sys/terrain_system.hpp>
#include <px/core/sys/body_system.hpp>
#include <px/core/sys/character_system.hpp>
#include <px/core/sys/behavior_system.hpp>

#include "input_adapter.hpp"

#include <px/rl/skill.hpp>

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

			terrain_director m_terrain_director;
			terrain m_terrain;

			environment m_environment;
			game m_game;
			factory m_factory;
			unsigned int m_last_turn;

		public:
			engine(shell::opengl* gl)
				: control_chain(m_ui, m_game_adapter, [this](point2 pixel) { return translate_canvas(pixel); }, [this](point2 pixel) { return translate_world(pixel); })
				, m_game_adapter(m_game)
				, m_renderer(gl)
				, m_space(space_start_width)
				, m_ui_system(m_canvas, m_ui)
				, m_location_system(m_space)
				, m_sprite_system(m_canvas)
				, m_terrain_system(m_canvas, m_terrain)
				, m_behavior_system(m_environment)
				, m_factory(m_sprite_system, m_location_system, m_body_system, m_character_system, m_behavior_system)
				, m_terrain_director(m_factory)
				, m_terrain(m_terrain_director)
				, m_environment(m_ui)
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

				m_environment.start(m_terrain, m_space, m_terrain_director);

				// player props
				auto weapon = std::make_shared<body_component::item_type>();
				weapon->add({ rl::effect::weapon_damage, 0x00, 50 });
				weapon->set_name("Copper Sword");
				weapon->set_tag("copper_sword");

				// make chest
				auto chest = m_factory.produce();
				chest->add_appearance('$', { 1, 1, 1 });
				chest->add_location({ 2, 2 });
				chest->add_body(100, 100);
				chest->add_container();
				// add
				m_terrain.add(chest->assemble(persistency::serialized));

				// make player
				auto task = m_factory.produce();
				auto img = task->add_appearance('@', { 1, 1, 1 });
				auto pawn = task->add_location({ 1, 1 });
				auto body = task->add_body(100, 100);
				auto character = task->add_character();
				// setup
				body->join_faction(1);
				body->equip_weapon(weapon);
				character->add_skill("melee");
				// add
				auto player = task->assemble(persistency::permanent);
				m_terrain.add(player);

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
				pos.mirror<1>(); // flip Y axis, ui grid grows down
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