// name: canvas.h
// type: c++
// desc: class declaration
// auth: isource

#ifndef PX_SHELL_CANVAS_HPP
#define PX_SHELL_CANVAS_HPP

#include <px/common/matrix.hpp>
#include <px/common/rectangle.hpp>
#include <px/shell/symbol.hpp>

namespace px
{
	namespace shell
	{
		class canvas : public matrix2<symbol>
		{
		private:
			color m_front;
			color m_back;

		public:
			canvas(point2::component x, point2::component y);
			virtual ~canvas();

		public:
			void text_color(color front_color);
			void draw_color(color back_color);

			void cls();
			void cls(const symbol &fill);
			void rectangle(const px::rectangle &bounds);
			void rectangle(const px::rectangle &bounds, const color &back);
			void pset(const point2 &position);
			void pset(const point2 &position, const color &back);

			void write(const point2 &position, symbol::code_t code);
			void write(const point2 &position, symbol::code_t code, const color &front);
			void write(point2 position, const std::string &text);
			void write(point2 position, const std::string &text, const color &front);
		};
	}
}

#endif