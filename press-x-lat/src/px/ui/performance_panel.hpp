// name: performance_panel.hpp
// type: c++
// desc: base class declaration
// auth: is0urce

// ui panel for displaying fps

#ifndef PX_UI_PERFORMANCE_PANEL_HPP
#define PX_UI_PERFORMANCE_PANEL_HPP

#include <px/common/fps_counter.hpp>
#include <px/ui/stack_panel.hpp>

#include <string>

namespace px
{
	namespace ui
	{
		class performance_panel : public stack_panel
		{
		public:
			performance_panel(fps_counter& fps) : m_fps(&fps) {}
			virtual ~performance_panel() {}

		protected:
			virtual void draw_panel(shell::canvas& cnv) const override
			{
				stack_panel::draw_panel(cnv);

				m_fps->frame_processed();

				cnv.write(start(), std::string("fps:") + std::to_string(m_fps->fps()));
			}

		private:
			fps_counter* m_fps;
		};
	}
}

#endif