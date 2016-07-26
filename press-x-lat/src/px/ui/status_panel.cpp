// name: status_panel.cpp
// type: c++
// desc: class
// auth: is0urce

#include "status_panel.hpp"

#include <px/core/environment.hpp>

#include <string>

namespace px
{
	namespace ui
	{
		void status_panel::draw_panel(shell::canvas& cnv) const
		{
			
			auto b = m_environment->traversable(m_hover, rl::traverse::floor);



			std::string msg = to_string(m_hover) + (b ? std::string(" - free - ") : std::string(" - BLOCKING"));

			//cnv.rectangle(win, color(0x0000ff));
			cnv.write(bounds().start(), msg);
		}
	}
}

