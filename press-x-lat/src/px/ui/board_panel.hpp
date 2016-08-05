// name: boadr_panel.hpp
// type: c++
// desc: base class declaration
// auth: is0urce

#ifndef PX_UI_BOARD_PANEL_HPP
#define PX_UI_DOARD_PANEL_HPP

#include <px/ui/stack_panel.hpp>

#include <px/common/rectangle.hpp>
#include <px/common/color.hpp>

namespace px
{
	namespace ui
	{
		class board_panel : public stack_panel
		{
		private:
			color m_color;

		public:
			board_panel(color bg) : m_color(bg) {}
			virtual ~board_panel() {}

		protected:
			virtual void draw_panel(shell::canvas& cnv) const override
			{
				stack_panel::draw_panel(cnv);

				cnv.rectangle(bounds(), m_color);
			}
		};
	}
}

#endif