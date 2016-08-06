// name: main_panel.hpp
// type: c++
// desc: base class declaration
// auth: is0urce

#ifndef PX_UI_MAIN_PANEL_HPP
#define PX_UI_MAIN_PANEL_HPP

#include <px/ui/stack_panel.hpp>
#include <px/shell/key.hpp>

namespace px
{
	using shell::key;
	namespace core
	{
		class main_panel : public ui::stack_panel
		{
		public:
			main_panel() {}
			virtual ~main_panel() {}

		protected:
			virtual bool key_control(shell::key code) override
			{
				switch (code)
				{
				case key::panel_inventory:
					disable("craft");
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