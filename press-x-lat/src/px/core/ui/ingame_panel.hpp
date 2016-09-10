// name: ingame.hpp
// type: c++
// desc: class declaration
// auth: is0urce

#ifndef PX_UI_INGAME_PANEL_HPP
#define PX_UI_INGAME_PANEL_HPP

#include <px/ui/stack_panel.hpp>
#include <px/shell/key.hpp>

namespace px
{
	using shell::key;
	namespace core
	{
		class ingame_panel : public ui::stack_panel
		{
		public:
			virtual ~ingame_panel() {}

		protected:
			virtual bool key_stacked(shell::key code) override
			{
				switch (code)
				{
				case key::panel_inventory:
					deactivate("craft");
					deactivate("map");
					reverse_toggle("inventory");
					return true;
				case key::panel_map:
					deactivate("craft");
					deactivate("inventory");
					reverse_toggle("map");
					return true;
				default:
					return false;
				}
			}
		};
	}
}

#endif