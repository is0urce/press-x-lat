// name: rendering_system.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_CORE_RENDERING_SYSTEM_HPP
#define PX_CORE_RENDERING_SYSTEM_HPP

#include <px/es/i_system.hpp>
#include <px/es/manager.hpp>
#include <px/shell/canvas.hpp>
#include <px/shell/renderer.hpp>
#include <px/shell/fps_counter.hpp>

#include "image_component.hpp"
#include "location_component.hpp"

namespace px
{
	namespace core
	{
		class rendering_system
			: public es::i_system
			, public es::manager<image_component, 10000>
			, public shell::renderer
		{
		private:
			shell::canvas m_canvas;
			shell::fps_counter m_fps;

		public:
			rendering_system(shell::opengl* ogl) : renderer(ogl), m_canvas(1, 1)
			{
			}
			virtual ~rendering_system() {}

		protected:
			virtual void update_system() override
			{
				m_fps.frame_processed();

				int w, h;
				canvas_size(w, h);
				m_canvas.resize(w, h);
				m_canvas.cls();
				m_canvas.write({ 0, 0 }, std::string("fps: ") + std::to_string(m_fps.fps()));

				enumerate([this](image_component &img)
				{
					if (img.active())
					{
						location_component* location = (location_component*)img;
						if (location)
						{
							m_canvas.write(location->current(), img.glyph, img.tint);
						}
					}
				});

				render(0, m_canvas);
			}

		public:
			auto make_sprite(unsigned int glyph)-> decltype(make())
			{
				auto sprite = make();
				sprite->glyph = glyph;
				return sprite;
			}
		};
	}
}

#endif