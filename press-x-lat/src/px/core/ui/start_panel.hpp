// name: start_panel.hpp
// type: c++
// desc: base class declaration
// auth: is0urce

// startion main menu title panel

#ifndef PX_UI_START_PANEL_HPP
#define PX_UI_START_PANEL_HPP

#include <px/ui/stack_panel.hpp>
#include <px/shell/key.hpp>

namespace px
{
	using shell::key;
	namespace core
	{
		class start_panel : public ui::stack_panel
		{
		public:
			start_panel() {}
			virtual ~start_panel() {}

		protected:
			virtual bool key_control(shell::key code) override
			{
				return stack_panel::key_control(code);
			}
		};
	}
}

#endif