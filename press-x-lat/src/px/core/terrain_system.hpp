// name: terrain_system.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_CORE_TERRAIN_SYSTEM_HPP
#define PX_CORE_TERRAIN_SYSTEM_HPP

#include <px/es/i_system.hpp>
#include <px/shell/canvas.hpp>
#include "scene.hpp"


namespace px
{
	namespace core
	{
		class terrain_system
			: public es::i_system
		{
		private:
			shell::canvas* m_canvas;
			scene* m_scene;
			std::shared_ptr<location_component> m_camera;

		public:
			terrain_system(shell::canvas& cnv, scene& s)
				: m_canvas(&cnv), m_scene(&s)
			{
			}
			virtual ~terrain_system()
			{
			}
			terrain_system(const terrain_system&) = delete;

		protected:
			virtual void update_system() override
			{
				int w = m_canvas->width();
				int h = m_canvas->height();
				point2 camera = m_camera ? m_camera->current() : point2(0, 0);

				point2 start = camera - point2(w / 2, h / 2);

				for (int j = 0; j < h; ++j)
				{
					for (int i = 0; i < w; ++i)
					{
						auto img = m_scene->select(start.moved(i, j)).appearance();
						m_canvas->write(point2(i, h - j - 1), img.glyph, img.tint);
					}
				}
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