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
#include <px/shell/fps_counter.hpp>

#include "unit.hpp"
#include "rendering_system.hpp"
#include "location_system.hpp"
#include "input_adapter.hpp"
#include "game.hpp"

#include <px/data/factory.hpp>

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
			shell::fps_counter m_fps;

			rendering_system m_rs;
			location_system m_ls;

			game m_game;
			unsigned int m_turn;

			data::factory m_factory;
			std::shared_ptr<unit> m_unit;

		public:
			engine(shell::opengl* gl)
				: control_dispatcher(m_adapter), m_adapter(m_game)
				, m_renderer(gl)
				, m_space(space_start_width)
				, m_canvas(1, 1)
				, m_ls(m_space)
				, m_rs(m_canvas)
				, m_factory(m_rs, m_ls)
				, m_turn(0)
			{
				add(&m_rs);
				add(&m_ls);

				auto task = m_factory.produce();
				auto a = task->add_appearance('@');
				auto l = task->add_location({ 0, 0 });
				a->tint = { 0, 1, 0 };
				m_unit = task->assemble();

				m_unit->activate();

				m_game.impersonate(l);
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
				m_canvas.resize(w, h);
				m_canvas.cls();
			}
			virtual void post_update_engine() override
			{
				m_fps.frame_processed();
				m_canvas.write({ 0, 0 }, std::string("fps: ") + std::to_string(m_fps.fps()));
				m_renderer.render(0, m_canvas);
			}
			virtual bool fixed() override
			{
				bool result = false;
				auto turn = m_game.time();
				if (m_turn != turn)
				{
					m_turn = turn;
					result = true;
				}
				return result;
			}
		};
	}
}

#endif