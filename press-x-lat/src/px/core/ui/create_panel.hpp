// name: create_panel.hpp
// type: c++
// desc: base class declaration
// auth: is0urce

#ifndef PX_UI_CREATE_PANEL_HPP
#define PX_UI_CREATE_PANEL_HPP

#include <px/ui/stack_panel.hpp>
#include <px/shell/key.hpp>

namespace px
{
	namespace core
	{
		class create_panel final : public ui::stack_panel
		{
		public:
			create_panel() {}
			virtual ~create_panel() {}

		protected:
			virtual bool key_control(shell::key code) override {}
			virtual void draw_panel(shell::canvas& cnv) const override {}
		};
	}
}

#endif