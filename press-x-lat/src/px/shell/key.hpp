// name: key.hpp
// type: c++ header
// desc: system virtual key enum
// auth: is0urce

// virtual keys used by px

#ifndef PX_SHELL_KEY_HPP
#define PX_SHELL_KEY_HPP

namespace px
{
	namespace shell
	{
		enum class key : unsigned int
		{
			not_valid,

			quick_save,
			quick_load,

			move_none,
			move_north,
			move_south,
			move_west,
			move_east,
			move_northwest,
			move_northeast,
			move_southwest,
			move_southeast,

			action_use,
			action_alt,

			action0,
			action1,
			action2,
			action3,
			action4,
			action5,
			action6,
			action7,
			action8,
			action9,

			navigate_next,
			navigate_previous,

			panel_inventory,
			panel_character,
			panel_map,
			panel_journal,

			command_ok,
			command_cancel,

			max_value = command_cancel
		};
	}
}

#endif