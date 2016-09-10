// name: status_panel.hpp
// type: c++
// desc: base class declaration
// auth: is0urce

#ifndef PX_UI_STATUS_PANEL_HPP
#define PX_UI_STATUS_PANEL_HPP

#include <px/ui/stack_panel.hpp>

#include <string>

namespace px
{
	namespace core
	{
		class environment;
		class status_panel : public ui::stack_panel
		{
		private:
			environment* m_environment;

		public:
			status_panel(environment& e);
			virtual ~status_panel();
			status_panel(const status_panel&) = delete;

		protected:
			virtual void draw_stacked(shell::canvas& cnv) const override;
		};
	}
}

#endif