// name: press_panel.hpp
// type: c++
// desc: template class
// auth: is0urce

#ifndef PX_UI_PRESS_PANEL_HPP
#define PX_UI_PRESS_PANEL_HPP

#include <px/ui/stack_panel.hpp>

#include <functional>

namespace px
{
	namespace ui
	{
		template <typename Press>
		class press_panel : public stack_panel
		{
		public:
			void set_press(Press fn)
			{
				m_press = fn;
			}
			bool call(point2 const& position, unsigned int v_button)
			{
				return m_press(position, v_button);
			}

		public:
			press_panel(const Press &button = Press()) : m_press(button) {}
			virtual ~press_panel() {}

		protected:
			virtual bool click_stacked(point2 const& position, unsigned int v_button) override
			{
				return bounds().contains(position) && m_press(position, v_button);
			}

		private:
			Press m_press;
		};
	}
}

#endif