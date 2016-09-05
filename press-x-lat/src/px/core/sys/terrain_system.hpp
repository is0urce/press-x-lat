// name: terrain_system.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_CORE_TERRAIN_SYSTEM_HPP
#define PX_CORE_TERRAIN_SYSTEM_HPP

#include <px/es/i_system.hpp>
#include <px/shell/canvas.hpp>
#include <px/core/environment.hpp>

namespace px
{
	namespace core
	{
		class terrain_system
			: public es::i_system
		{
		private:
			shell::canvas* m_canvas;
			environment* m_environment;
			std::shared_ptr<location_component> m_camera;

		public:
			terrain_system(shell::canvas& cnv)
				: m_canvas(&cnv), m_environment(nullptr)
			{
			}
			terrain_system(shell::canvas& cnv, environment& env) : terrain_system(cnv)
			{
				bind(env);
			}
			virtual ~terrain_system()
			{
			}
			terrain_system(const terrain_system&) = delete;

		protected:
			virtual void update_system() override
			{
				if (m_canvas && m_environment && m_camera)
				{
					if (terrain const* scene = m_environment->scene())
					{
						m_canvas->enumerate([&, camera = m_camera->current(), center = m_canvas->range() / 2](point2 const& location, auto &symbol) {

							auto relative = location - center;
							relative.mirror<1>();

							auto &img = scene->select(camera + relative).appearance();

							symbol.code = img.glyph;
							symbol.front = img.tint;
							symbol.back = img.bg;
						});
					}
				}
			}
			virtual void fixed_update_system() override
			{
				m_environment->scene()->handle_units();
			}

		public:
			void bind(environment &terra)
			{
				m_environment = &terra;
			}
			void tear()
			{
				m_environment = nullptr;
			}
			void focus_camera(std::shared_ptr<location_component> camera)
			{
				m_camera = camera;
			}
		};
	}
}

#endif