// name: engine.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_CORE_ENGINE_HPP
#define PX_CORE_ENGINE_HPP

#include <px/common/toggle.hpp>
#include <px/es/i_engine.hpp>
#include <px/shell/control.hpp>
#include <px/shell/control_dispatcher.hpp>
#include <px/shell/opengl.h>
#include <px/shell/renderer.hpp>

#include "rendering_system.hpp"
#include "location_system.hpp"
#include "terrain_system.hpp"
#include "body_system.hpp"
#include "character_system.hpp"

#include "unit.hpp"
#include "game.hpp"
#include "terrain.hpp"

#include "input_adapter.hpp"

#include <px/data/factory.hpp>
#include <px/rl/ability.hpp>

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
			, public shell::control_dispatcher
			, public toggle<true>
		{
		private:
			input_adapter m_adapter;

			qtree<location_component*> m_space;
			shell::renderer m_renderer;
			shell::canvas m_canvas;

			location_system m_ls;
			rendering_system m_sprite_system;
			terrain_system m_terrain_system;
			body_system m_body_system;
			character_system m_char_system;

			terrain m_terrain;
			environment m_environment;
			game m_game;
			unsigned int m_last_turn;

			data::factory m_factory;

		public:
			engine(shell::opengl* gl)
				: control_dispatcher(m_adapter), m_adapter(m_game)
				, m_renderer(gl)
				, m_space(space_start_width)
				, m_canvas(1, 1)
				, m_ls(m_space)
				, m_sprite_system(m_canvas)
				, m_terrain_system(m_canvas, m_terrain)
				, m_factory(m_sprite_system, m_ls, m_body_system, m_char_system)
				, m_environment(m_terrain, m_space)
				, m_game(m_environment)
				, m_last_turn(0)
			{
				add(&m_body_system);
				add(&m_char_system);
				add(&m_ls);
				add(&m_terrain_system);
				add(&m_sprite_system);

				auto weapon = std::make_shared<rl::item<rl::effect>>();
				weapon->emplace(rl::effect::weapon_damage, 10);

				auto task = m_factory.produce();
				auto sprite = task->add_appearance('@');
				auto l = task->add_location({ 1, 1 });
				auto body = task->add_body(100);
				auto character = task->add_character();

				body->equip_weapon(weapon);
				sprite->tint = { 0, 1, 1 };
				character->add_skill(character_component::skill::create_target([this](auto ch, auto& bd) { m_environment.ui().toggle("performance"); }, nullptr));

				m_terrain.add(task->assemble());

				m_environment.impersonate(l);
				m_sprite_system.focus_camera(l);
				m_terrain_system.focus_camera(l);
			}
			virtual ~engine()
			{
			}
			engine(const engine&) = delete;

		protected:
			virtual void pre_update_engine() override
			{
				int w, h;
				m_renderer.canvas_size(w, h);
				if (w > 0 && h > 0)
				{
					m_canvas.resize(w, h);
					m_environment.ui().layout(m_canvas.range());
				}
				m_canvas.cls();
			}
			virtual void post_update_engine() override
			{
				m_environment.ui().draw(m_canvas);
				m_renderer.render(0, m_canvas);
			}
			virtual bool fixed() override
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