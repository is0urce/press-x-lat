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
			typedef press_panel<press_fn> press_type;

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
			void assign_key(shell::key code)
			{
				assign_key(code, 0);
			}
			void assign_key(shell::key code, unsigned int v_button)
			{
				m_key = true;
				m_code = code;
				m_vbutton = v_button;
			}
			void clear_key()
			{
				m_key = false;
			}


		public:
			button(color front, color hover, std::string text, color text_color, press_fn button)
				: m_front_color(front), m_hover_color(hover), m_key(false)
			{
				if (!button) throw std::runtime_error("px::ui::button::button(..) press function is null");

				m_text = emplace<ui::text>(alignment::fill(), text, text_color).get();
				m_press = emplace<press_type>(alignment::fill(), button).get();
			}
			button(color front, color hover, press_fn button)
				: button(front, hover, "", color(), button)
			{
			}
			button(press_fn button)
				: button(color(), color(), "", color(), button)
			{
			}
			virtual ~button() {}

		protected:
			virtual bool hover_stacked(point2 const& position) override
			{
				m_hover = position;
				return bounds().contains(position);
			}
			virtual void draw_stacked(shell::canvas& cnv) const override
			{
				cnv.rectangle(bounds(), bounds().contains(m_hover) ? m_hover_color : m_front_color);
			}
			virtual bool key_stacked(shell::key code) override
			{
				return m_key && code == m_code && m_press->call(m_hover, m_vbutton);
			}

		private:
			ui::text* m_text;
			press_type* m_press;
			point2 m_hover;
			color m_front_color;
			color m_hover_color;

			bool m_key;
			shell::key m_code;
			unsigned int m_vbutton;
		};
	}
}

#endif