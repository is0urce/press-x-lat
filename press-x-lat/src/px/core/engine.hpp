// name: engine.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_CORE_ENGINE_HPP
#define PX_CORE_ENGINE_HPP

#include <px/common/toggle.hpp>

#include <px/es/i_engine.hpp>
#include <px/ui/stack_panel.hpp>

#include <px/shell/control.hpp>
#include <px/shell/control_chain.hpp>
#include <px/shell/opengl.h>
#include <px/shell/renderer.hpp>

#include <px/core/sys/rendering_system.hpp>
#include <px/core/sys/ui_system.hpp>
#include <px/core/sys/location_system.hpp>
#include <px/core/sys/terrain_system.hpp>
#include <px/core/sys/body_system.hpp>
#include <px/core/sys/character_system.hpp>
#include <px/core/sys/behavior_system.hpp>

#include <px/core/data/factory.hpp>

#include <px/core/game.hpp>
#include <px/core/input_adapter.hpp>


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
		public:
			engine(shell::opengl* gl)
				: control_chain(m_ui, m_game_adapter, [this](point2 const& pixel) { return translate_canvas(pixel); }, [this](point2 const& pixel) { return translate_world(pixel); })
				, m_game(m_environment)
				, m_game_adapter(m_game)
				, m_last_turn(0)
				, m_renderer(gl)
				, m_space(space_start_width)
				, m_ui_system(m_canvas, m_ui)
				, m_location_system(m_space)
				, m_sprite_system(m_canvas)
				, m_terrain_system(m_canvas, m_environment)
				, m_behavior_system(m_environment)
				, m_character_system(m_environment)
				, m_factory(m_sprite_system, m_location_system, m_body_system, m_character_system, m_behavior_system)
				, m_environment(m_ui, m_factory, m_space)
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

				m_environment.start();
			}
			virtual ~engine() {}
			engine(engine const&) = delete;
			engine& operator=(engine const&) = delete;

		private:
			point2 translate_canvas(point2 const& pixel) const
			{
				return m_renderer.translate_canvas(pixel);
			}
			point2 translate_world(point2 const& pixel) const
			{
				auto pos = translate_canvas(pixel) - (m_canvas.range() / 2);
				pos.mirror<1>(); // flip Y axis, ui grid grows down
				auto camera = m_environment.player();
				return pos + (camera ? camera->current() : point2(0, 0));
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

				if (!m_environment.active())
				{
					deactivate();
				}
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

		private:
			qtree<location_component*> m_space;
			shell::renderer m_renderer;
			shell::canvas m_canvas;
			ui::stack_panel m_ui;

			location_system m_location_system;
			rendering_system m_sprite_system;
			terrain_system m_terrain_system;
			body_system m_body_system;
			character_system m_character_system;
			behavior_system m_behavior_system;
			ui_system m_ui_system;

			factory m_factory;
			environment m_environment;
			input_adapter m_game_adapter;
			game m_game;

			unsigned int m_last_turn;
		};
	}
}

#endif