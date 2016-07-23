#pragma once
// name: performance_panel.hpp
// type: c++
// desc: base class declaration
// auth: is0urce

#ifndef PX_UI_PERFORMANCE_PANEL_HPP
#define PX_UI_PERFORMANCE_PANEL_HPP

#include "stack_panel.hpp"

#include <px/shell/fps_counter.hpp>
#include <px/ui/stack_panel.hpp>

#include <string>

namespace px
{
	namespace ui
	{
		class performance_panel : public stack_panel
		{
		private:
			shell::fps_counter* m_fps;
		public:
			performance_panel(shell::fps_counter& fps) : m_fps(&fps) {}
			virtual ~performance_panel() {}

		protected:
			virtual void draw_panel(shell::canvas& cnv) const override
			{
				m_fps->frame_processed();

				std::string str("fps:");
				str += std::to_string(m_fps->fps());
				auto win = bounds();
				cnv.rectangle(win, color(0x0000ff));
				cnv.write(win.start(), str);
			}
		};
	}
}

#endif