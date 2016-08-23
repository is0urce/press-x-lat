// name: title_panel.hpp
// type: c++
// desc: base class declaration
// auth: is0urce

#ifndef PX_UI_TITLE_PANEL_HPP
#define PX_UI_TITLE_PANEL_HPP

#include <px/ui/stack_panel.hpp>
#include <px/shell/key.hpp>

namespace px
{
	using shell::key;
	namespace core
	{
		class title_panel final : public ui::stack_panel
		{
		public:
			title_panel();
			virtual ~title_panel();

		protected:
			virtual bool key_control(shell::key code) override;
			virtual void draw_panel(shell::canvas& cnv) const override;
		};
	}
}

#endif