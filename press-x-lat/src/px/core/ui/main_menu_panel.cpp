// name: main_menu_panel.cpp
// type: c++
// desc: class methods implementation
// auth: is0urce

#include "main_menu_panel.hpp"

namespace px
{
	using shell::key;
	namespace core
	{
		main_menu_panel::main_menu_panel() {}
		main_menu_panel::~main_menu_panel() {}

			bool main_menu_panel::key_control(shell::key code)
			{
				return stack_panel::key_control(code);
			}
			void main_menu_panel::draw_panel(shell::canvas& cnv) const
			{

			}
	}
}