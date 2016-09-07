// name: button_panel.hpp
// type: c++
// desc: template class
// auth: is0urce

#ifndef PX_UI_BUTTON_HPP
#define PX_UI_BUTTON_HPP

#include <px/common/color.hpp>

#include <px/ui/stack_panel.hpp>

#include <px/ui/press_panel.hpp>
#include <px/ui/text.hpp>

#include <functional>
#include <string>

namespace px
{
	namespace ui
	{
		class button : public stack_panel
		{
		public:
			typedef std::function<bool(point2 const&, unsigned int)> press_fn;

		public:
			void set_hover_color(color c)
			{
				m_hover_color = c;
			}
			void set_front_color(color c)
			{
				m_front_color = c;
			}
			ui::text* text()
			{
				return m_text;
			}


		public:
			button(color front, color hover, std::string text, color text_color, press_fn button)
				: m_front_color(front), m_hover_color(hover), m_hover(false)
			{
				if (!button) throw std::runtime_error("px::ui::button::button(..) press function is null");

				m_text = emplace<ui::text>(alignment::fill(), text, text_color).get();
				emplace<press_panel<press_fn>>(alignment::fill(), button);
			}
			virtual ~button() {}

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