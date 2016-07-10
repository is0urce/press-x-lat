// name: control_dispatcher.hpp
// type: c++ header
// desc: class declaration
// auth: is0urce

// send input 1:1 to other control

#ifndef PX_CONTROL_DISPATCHER_HPP
#define PX_CONTROL_DISPATCHER_HPP

#include "control.hpp"

namespace px
{
	namespace shell
	{
		class control_dispatcher : public control
		{
		private:
			control* m_control;
		public:
			control_dispatcher(control& ctrl) : m_control(&ctrl)
			{
			}
			virtual ~control_dispatcher() {}

		protected:
			virtual bool key_control(key_t code) override { return m_control->key(code); }
			virtual bool hover_control(const point2 &position) override { return m_control->hover(position); }
			virtual bool click_control(const point2 &position, unsigned int button) override { return m_control->click(position, button); }
			virtual bool scroll_control(int delta) override { return m_control->scroll(delta); }
		};
	}
}

#endif