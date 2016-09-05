// name: static_text_panel.hpp
// type: c++
// desc: base class declaration
// auth: is0urce

#ifndef PX_UI_STATIC_TEXT_PANEL_HPP
#define PX_UI_STATIC_TEXT_PANEL_HPP

#include <px/ui/stack_panel.hpp>
#include <px/ui/text_alignment.hpp>

#include <px/common/color.hpp>

#include <string>

namespace px
{
	namespace ui
	{
		class static_text_panel : public stack_panel
		{
		public:
			void set_text(std::string text)
			{
				m_text = text;
				m_len = text.size();
			}
			void set_text(std::string text, color c)
			{
				set_text(text);
				m_color = c;
			}
			void set_text_color(color c)
			{
				m_color = c;
			}
			void set_text_alignment(text_alignment align)
			{
				m_align = align;
			}

		public:
			static_text_panel(std::string text, color c)
				: m_color(c), m_align(text_alignment::left)
			{
				set_text(text);
			}
			virtual ~static_text_panel() {}

		protected:
			virtual void draw_panel(shell::canvas& cnv) const override
			{
				stack_panel::draw_panel(cnv);

				point2 pen = start();
				switch (m_align)
				{
				case text_alignment::right:
					pen.move_axis<0>(bounds().range().x() - m_len);
					break;
				case text_alignment::center:
					pen.move_axis<0>((bounds().range().x() - m_len) / 2);
					break;
				case text_alignment::left:
					break;
				}
				cnv.write(pen, m_text, m_color);
			}

		private:
			std::string m_text;
			unsigned int m_len;
			text_alignment m_align;
			color m_color;
		};
	}
}

#endif