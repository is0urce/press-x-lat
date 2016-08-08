// name: button_panel.hpp
// type: c++
// desc: base class declaration
// auth: is0urce

#ifndef PX_UI_BUTTON_PANEL_HPP
#define PX_UI_BUTTON_PANEL_HPP

#include <px/ui/stack_panel.hpp>

namespace px
{
	namespace ui
	{
		struct nop_button
		{
			bool operator()(unsigned int) { return false; }
		};

		// Button = bool operator()(unsigned int virtual_button)
		template <typename Button = nop_button>
		class button_panel : public stack_panel
		{
		protected:
			virtual bool click_control(const point2 &position, unsigned int v_button) override
			{
				if (bounds().contains(position))
				{
					return m_button(v_button);
				}
				return stack_panel::click_control(position, v_button);
			}

		public:
			button_panel(const Button &button = Button()) : m_button(button) {}
			virtual ~button_panel() {}

		private:
			Button m_button;
		};
	}
}

#endif