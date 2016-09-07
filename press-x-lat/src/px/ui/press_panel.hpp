// name: press_panel.hpp
// type: c++
// desc: template class
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
			bool operator()(point2 const&, unsigned int) { return false; }
		};

		// Press : bool(unsigned int)
		template <typename Press = nop_press>
		class press_panel : public stack_panel
		{
		public:
			void set_press(Press fn)
			{
				m_press = fn;
			}

		public:
			press_panel(const Press &button = Press()) : m_press(button) {}
			virtual ~press_panel() {}

		protected:
			virtual bool click_control(point2 const& position, unsigned int v_button) override
			{
				if (bounds().contains(position))
				{
					return m_press(position, v_button);
				}
				return stack_panel::click_control(position, v_button);
			}

		private:
			Press m_press;
		};
	}
}

#endif