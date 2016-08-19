// name: terrain_system.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_CORE_TERRAIN_SYSTEM_HPP
#define PX_CORE_TERRAIN_SYSTEM_HPP

#include <px/es/i_system.hpp>
#include <px/shell/canvas.hpp>
#include <px/core/terrain.hpp>

namespace px
{
	namespace core
	{
		class terrain_system
			: public es::i_system
		{
		private:
			shell::canvas* m_canvas;
			terrain* m_terrain;
			std::shared_ptr<location_component> m_camera;

		public:
			terrain_system(shell::canvas& cnv, terrain& terra)
				: m_canvas(&cnv), m_terrain(&terra)
			{
			}
			virtual ~terrain_system()
			{
			}
			terrain_system(const terrain_system&) = delete;

		protected:
			virtual void update_system() override
			{
				int w = static_cast<int>(m_canvas->width());
				int h = static_cast<int>(m_canvas->height());
				point2 camera = m_camera ? m_camera->current() : point2(0, 0);

				point2 start = camera - point2(w / 2, h / 2);

				m_canvas->enumerate([&](point2 const& location, auto const& symbol) {
					auto img = m_terrain->select(start.moved(location)).appearance();
					point2 position(location.x(), h - location.y() - 1);
					m_canvas->write(position, img.glyph, img.tint);
					m_canvas->pset(position, img.bg);
				});
				//for (int j = 0; j < h; ++j)
				//{
				//	for (int i = 0; i < w; ++i)
				//	{
				//		auto img = m_terrain->select(start.moved(i, j)).appearance();
				//		point2 position(i, h - j - 1);
				//		m_canvas->write(position, img.glyph, img.tint);
				//		m_canvas->pset(position, img.bg);
				//	}
				//}
			}
			virtual void fixed_update_system() override
			{
				m_terrain->handle_units();
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