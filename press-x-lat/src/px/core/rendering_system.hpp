// name: rendering_system.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_CORE_RENDERING_SYSTEM_HPP
#define PX_CORE_RENDERING_SYSTEM_HPP

#include <px/es/i_system.hpp>
#include <px/es/manager.hpp>
#include <px/shell/canvas.hpp>

#include "image_component.hpp"
#include "location_component.hpp"

namespace px
{
	namespace core
	{
		class rendering_system
			: public es::i_system
			, public es::manager<image_component, 10000>
		{
		private:
			shell::canvas* m_canvas;
			std::shared_ptr<location_component> m_camera;

		public:
			rendering_system(shell::canvas& cnv)
				: m_canvas(&cnv)
			{
			}
			virtual ~rendering_system()
			{
			}

		protected:
			virtual void update_system() override
			{
				int w = m_canvas->width() / 2;
				int h = m_canvas->height() / 2;
				point2 camera = m_camera ? m_camera->current(): point2(0, 0);
				enumerate([this,camera,w,h](const image_component &img)
				{
					if (img.active())
					{
						if (location_component* location = img)
						{
							auto relative = location->current() - camera;

							m_canvas->write({ w + relative.x(), h - relative.y() }, img.glyph, img.tint);
						}
					}
				});
			}

		public:
			void focus_camera(std::shared_ptr<location_component> camera)
			{
				m_camera = camera;
			}
		};
	}
}

#endif