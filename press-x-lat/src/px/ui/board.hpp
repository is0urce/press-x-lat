// name: boadr_panel.hpp
// type: c++
// desc: base class declaration
// auth: is0urce

#ifndef PX_UI_BOARD_HPP
#define PX_UI_BOARD_HPP

#include <px/ui/stack_panel.hpp>

#include <px/common/color.hpp>

namespace px
{
	namespace ui
	{
		class board : public stack_panel
		{
		public:
			void set_color(color bg)
			{
				m_color = bg;
			}

		public:
			board(color bg) : m_color(bg) {}
			virtual ~board() {}

		protected:
			virtual void draw_panel(shell::canvas& cnv) const override
			{
				stack_panel::draw_panel(cnv);

				cnv.rectangle(bounds(), m_color);
			}

		private:
			color m_color;
		};
	}
}

#endif