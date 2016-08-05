// name: target_panel.hpp
// type: c++
// desc: base class declaration
// auth: is0urce

#ifndef PX_UI_TARGET_PANEL_HPP
#define PX_UI_TARGET_PANEL_HPP

#include <px/ui/stack_panel.hpp>

#include <string>

namespace px
{
	namespace core
	{
		class environment;
		class target_panel : public ui::stack_panel
		{
		private:
			environment* m_environment;

		public:
			target_panel(environment& e);
			virtual ~target_panel();

		protected:
			virtual void draw_panel(shell::canvas& cnv) const override;
		};
	}
}

#endif