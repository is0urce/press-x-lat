// name: stack_panel.hpp
// type: c++
// desc: base class declaration
// auth: is0urce

#ifndef PX_UI_STACK_PANEL_HPP
#define PX_UI_STACJ_PANEL_HPP

#include <px/ui/alignment.hpp>
#include "panel.hpp"

namespace px
{
	namespace ui
	{
		class stack_panel : public panel
		{
		public:
			stack_panel() {}
			virtual ~stack_panel() {}

		protected:
			virtual void draw_panel(shell::canvas& cnv) override {}
		};
	}
}

#endif