// name: panel.hpp
// type: c++
// desc: base class declaration
// auth: is0urce

#ifndef PX_UI_PANEL_HPP
#define PX_UI_PANEL_HPP

#include <px/common/toggle.hpp>
#include <px/shell/control.hpp>
#include <px/shell/canvas.hpp>

namespace px
{
	namespace ui
	{
		class panel : public shell::control, public toggle<true>
		{
		public:
			panel() {}
			virtual ~panel() = 0 {}

		protected:
			virtual void draw_panel(shell::canvas& cnv) const = 0;

		public:
			void draw(shell::canvas& cnv) const { draw_panel(cnv); }
		};
	}
}

#endif