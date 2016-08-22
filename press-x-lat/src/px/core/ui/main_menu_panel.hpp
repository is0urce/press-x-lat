// name: main_menu_panel.hpp
// type: c++
// desc: base class declaration
// auth: is0urce

#ifndef PX_UI_MAIN_MENU_PANEL_HPP
#define PX_UI_MAIN_MENU_PANEL_HPP

#include <px/ui/stack_panel.hpp>
#include <px/shell/key.hpp>

namespace px
{
	using shell::key;
	namespace core
	{
		class main_menu_panel : public ui::stack_panel
		{
		public:
			main_menu_panel();
			virtual ~main_menu_panel();

		protected:
			virtual bool key_control(shell::key code) override;
			virtual void draw_panel(shell::canvas& cnv) const override;
		};
	}
}

#endif