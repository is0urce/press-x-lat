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
			typedef key key_type;

		protected:
			virtual bool key_control(key_type) { return false; }
			virtual bool hover_control(point2 const&) { return false; }
			virtual bool click_control(point2 const&, unsigned int) { return false; }
			virtual bool scroll_control(int) { return false; }

		public:
			bool key(key_type code) { return key_control(code); }
			bool hover(const point2 &position) { return hover_control(position); }
			bool click(const point2 &position, unsigned int button) { return click_control(position, button); }
			bool scroll(int delta) { return scroll_control(delta); }

		public:
			virtual ~control() {}
		};
	}
}

#endif