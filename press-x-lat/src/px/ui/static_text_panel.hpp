// name: static_text_panel.hpp
// type: c++
// desc: base class declaration
// auth: is0urce

#ifndef PX_UI_STATIC_TEXT_PANEL_HPP
#define PX_UI_STATIC_TEXT_PANEL_HPP

#include <px/ui/stack_panel.hpp>

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
			}
			void set_text(std::string text, color c)
			{
				m_text = text;
				m_color = c;
			}
			void set_text_color(color c)
			{
				m_color = c;
			}

		public:
			static_text_panel(std::string text, color c) : m_text(text), m_color(c) {}
			virtual ~static_text_panel() {}

		protected:
			virtual void draw_panel(shell::canvas& cnv) const override
			{
				stack_panel::draw_panel(cnv);
				cnv.write(start(), m_text, m_color);
			}

		private:
			std::string m_text;
			color m_color;
		};
	}
}

#endif