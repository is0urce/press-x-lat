// name: text_panel.hpp
// type: c++
// desc: base class declaration
// auth: is0urce

#ifndef PX_UI_TEXT_PANEL_HPP
#define PX_UI_TEXT_PANEL_HPP

#include <px/ui/stack_panel.hpp>

#include <px/common/color.hpp>

#include <string>

namespace px
{
	namespace ui
	{
		namespace
		{
			struct white_color_provider
			{
				color operator()() const { return color::white(); }
			};
		}
		template <typename TextProvider, typename ColorProvider = white_color_provider>
		class text_panel : public stack_panel
		{
		private:
			TextProvider m_text;
			ColorProvider m_color;

		public:
			text_panel(const TextProvider &text = TextProvider(), const ColorProvider &c = ColorProvider()) : m_text(text), m_color(c) {}
			virtual ~text_panel() {}

		protected:
			virtual void draw_panel(shell::canvas& cnv) const override
			{
				stack_panel::draw_panel(cnv);
				cnv.write(start(), m_text(), m_color());
			}
		};
	}
}

#endif