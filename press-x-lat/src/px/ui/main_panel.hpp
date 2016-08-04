// name: main_panel.hpp
// type: c++
// desc: base class declaration
// auth: is0urce

#ifndef PX_UI_MAIN_PANEL_HPP
#define PX_UI_MAIN_PANEL_HPP

#include "stack_panel.hpp"
#include <px/shell/key.hpp>

namespace px
{
	namespace ui
	{
		class main_panel : public stack_panel
		{
		public:
			main_panel() {}
			virtual ~main_panel() {}

		protected:
			virtual bool key_control(shell::key code) override
			{
				switch (code)
				{
				case shell::key::panel_inventory:
					toggle("inventory");
					break;
				default:
					break;
				}
				return stack_panel::key_control(code);
			}
		};
	}
}

#endif