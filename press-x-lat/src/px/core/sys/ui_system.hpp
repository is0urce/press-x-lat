// name: ui_system.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_CORE_UI_SYSTEM_HPP
#define PX_CORE_UI_SYSTEM_HPP


#include <px/es/i_system.hpp>
#include <px/shell/canvas.hpp>
#include <px/ui/stack_panel.hpp>


namespace px
{
	namespace core
	{
		class ui_system
			: public es::i_system
		{
		private:
			shell::canvas* m_canvas;
			ui::stack_panel* m_ui;

		public:
			ui_system(shell::canvas& cnv, ui::stack_panel &ui)
				: m_canvas(&cnv), m_ui(&ui)
			{
			}
			virtual ~ui_system()
			{
			}
			ui_system(const ui_system&) = delete;

		protected:
			virtual void update_system() override
			{
				m_ui->draw(*m_canvas);
			}
		};
	}
}

#endif