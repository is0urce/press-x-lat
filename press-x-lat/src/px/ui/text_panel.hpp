// name: text_panel.hpp
// type: c++
// desc: base class declaration
// auth: is0urce

#ifndef PX_UI_TEXT_PANEL_HPP
#define PX_UI_TEXT_PANEL_HPP

#include <px/ui/stack_panel.hpp>

#include <string>

namespace px
{
	namespace ui
	{
		class text_panel : public stack_panel
		{
		private:
			std::string m_text;

		public:
			text_panel(std::string text) : m_text(text) {}
			virtual ~text_panel() {}

		protected:
			virtual void draw_panel(shell::canvas& cnv) const override
			{
				stack_panel::draw_panel(cnv);
				cnv.write(bounds().start(), m_text);
			}
		};
	}
}

#endif