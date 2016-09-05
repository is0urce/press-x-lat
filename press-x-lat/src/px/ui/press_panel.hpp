// name: button_panel.hpp
// type: c++
// desc: base class declaration
// auth: is0urce

#ifndef PX_UI_PRESS_PANEL_HPP
#define PX_UI_PRESS_PANEL_HPP

#include <px/ui/stack_panel.hpp>

namespace px
{
	namespace ui
	{
		struct nop_press
		{
			bool operator()(unsigned int) { return false; }
		};

		// Button = bool operator()(unsigned int virtual_button)
		template <typename Press = nop_press>
		class press_panel : public stack_panel
		{
		protected:
			virtual bool click_control(point2 const& position, unsigned int v_button) override
			{
				if (bounds().contains(position))
				{
					return m_press(v_button);
				}
				return stack_panel::click_control(position, v_button);
			}

		public:
			press_panel(const Press &button = Press()) : m_press(button) {}
			virtual ~press_panel() {}

		private:
			Press m_press;
		};
	}
}

#endif