// name: control.hpp
// type: c++ header
// desc: interface declaration
// auth: is0urce

// intefcace for control in main mvc pattern
// functions return true if interaction handled

#ifndef PX_CONTROL_HPP
#define PX_CONTROL_HPP

#include <px/common/point.hpp>

namespace px
{
	namespace shell
	{
		enum class key : unsigned int;
		class control
		{
		public:
			typedef key key_t;
		public:
			virtual ~control() {}

		protected:
			virtual bool key_control(key_t code) { return false; }
			virtual bool hover_control(const point2 &position) { return false; }
			virtual bool click_control(const point2 &position, unsigned int button) { return false; }
			virtual bool scroll_control(int delta) { return false; }

		public:
			bool key(key_t code) { return key_control(code); }
			bool hover(const point2 &position) { return hover_control(position); }
			bool click(const point2 &position, unsigned int button) { return click_control(position, button); }
			bool scroll(int delta) { return scroll_control(delta); }
		};
	}
}

#endif