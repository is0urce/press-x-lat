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
	}
	namespace ui
	{
		class status_panel : public stack_panel
		{
		private:
			core::environment* m_environment;
			point2 m_hover;

		public:
			status_panel(core::environment& e) : m_environment(&e) {}
			virtual ~status_panel() {}

		protected:
			virtual void draw_panel(shell::canvas& cnv) const override;
			virtual bool hover_control(const point2 &position) override
			{
				m_hover = position;
				return false;
			}
			virtual bool click_control(const point2 &position, unsigned int v_button) override
			{
				m_hover = position;
				return false;
			}
		};
	}
}

#endif