// name: title_panel.cpp
// type: c++
// desc: class methods implementation
// auth: is0urce

#include "title_panel.hpp"

namespace px
{
	using shell::key;
	namespace core
	{
		title_panel::title_panel() {}
		title_panel::~title_panel() {}

		bool title_panel::key_control(shell::key code)
		{
			return stack_panel::key_control(code);
		}
		void title_panel::draw_panel(shell::canvas& cnv) const
		{
		}
	}
}