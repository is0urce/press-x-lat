// name: control_dispatcher.hpp
// type: c++ header
// desc: class declaration
// auth: is0urce

// send input 1:1 to other control

#ifndef PX_CONTROL_DISPATCHER_HPP
#define PX_CONTROL_DISPATCHER_HPP

#include "control.hpp"

#include <functional>

namespace px
{
	namespace shell
	{
		class control_dispatcher : public control
		{
		public:
			typedef std::function<point2(point2)> map_fn;
		private:
			control* m_control;
			map_fn m_map;
		public:
			control_dispatcher(control& ctrl, map_fn map)
				: m_control(&ctrl), m_map(map)
			{
			}
			virtual ~control_dispatcher() {}

		protected:
			virtual bool key_control(key_type code) override { return m_control->key(code); }
			virtual bool hover_control(const point2 &position) override { return m_control->hover(m_map(position)); }
			virtual bool click_control(const point2 &position, unsigned int button) override { return m_control->click(m_map(position), button); }
			virtual bool scroll_control(int delta) override { return m_control->scroll(delta); }
		};
	}
}

#endif