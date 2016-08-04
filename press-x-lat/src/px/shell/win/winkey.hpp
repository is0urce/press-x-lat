// name: bindings.hpp
// type: c++ header
// desc: class
// auth: is0urce

// key bindings for windows

#ifndef PX_SHELL_WIN_KEY_HPP
#define PX_SHELL_WIN_KEY_HPP

#include <px/shell/bindings.hpp>
#include <px/shell/key.hpp>

#include <windows.h>

namespace px
{
	namespace shell
	{
		class winkey : public bindings<WPARAM, key>
		{
		public:
			winkey()
			{
				bind('W', VK_UP, VK_NUMPAD8, key::move_north);
				bind('A', VK_LEFT, VK_NUMPAD4, key::move_west);
				bind('S', VK_DOWN, VK_NUMPAD2, key::move_south);
				bind('D', VK_RIGHT, VK_NUMPAD6, key::move_east);
				bind(VK_END, VK_NUMPAD1, key::move_southwest);
				bind(VK_NEXT, VK_NUMPAD3, key::move_southeast);
				bind(VK_HOME, VK_NUMPAD7, key::move_northwest);
				bind(VK_PRIOR, VK_NUMPAD9, key::move_northeast);
				bind(VK_SPACE, VK_NUMPAD5, key::move_none);

				bind('1', key::action1);
				bind('2', key::action2);
				bind('3', key::action3);
				bind('4', key::action4);
				bind('5', key::action5);
				bind('6', key::action6);
				bind('7', key::action7);
				bind('8', key::action8);
				bind('9', key::action9);
				bind('0', key::action0);
				bind('E', key::action_use);

				bind(VK_F5, key::quick_save);
				bind(VK_F9, key::quick_load);

				bind(VK_RETURN, key::command_ok);
				bind(VK_ESCAPE, key::command_cancel);

				bind('I', key::panel_inventory);
			}
			virtual ~winkey()
			{
			}
			winkey::v_key select(sys_key key)
			{
				return bindings::select(key, key::not_valid);
			}
		};
	}
}

#endif