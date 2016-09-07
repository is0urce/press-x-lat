// name: button_panel.hpp
// type: c++
// desc: template class
// auth: is0urce

#ifndef PX_UI_BUTTON_PANEL_HPP
#define PX_UI_BUTTON_PANEL_HPP

#include <px/common/color.hpp>

#include <px/ui/stack_panel.hpp>

#include <px/ui/press_panel.hpp>
#include <px/ui/text.hpp>

#include <string>

namespace px
{
	namespace ui
	{
		template <typename Press = nop_press>
		class button_panel : public stack_panel
		{
		public:
			void set_hover_color(color c)
			{
				m_hover = c;
			}
			void set_front_color(color c)
			{
				m_front = c;
			}
			text* text()
			{
				return m_text;
			}


		public:
			button_panel(color front, color hover, std::string text, color text_color, const Press &button = Press())
				: m_front_color(front), m_hover_color(hover), m_hover(false)
			{
				m_text = emplace<ui::text>(alignment::fill(), text, text_color).get();
				emplace<press_panel<Press>>(alignment::fill(), button);
			}
			virtual ~button_panel() {}

		protected:
			virtual bool hover_control(point2 const& position) override
			{
				m_hover = bounds().contains(position);
				return stack_panel::hover_control(position);
			}
			virtual void draw_panel(shell::canvas& cnv) const override
			{
				cnv.rectangle(bounds(), m_hover ? m_hover_color : m_front_color);

				stack_panel::draw_panel(cnv);
			}

		private:
			ui::text* m_text;
			bool m_hover;
			color m_front_color;
			color m_hover_color;
		};
	}
}

#endif