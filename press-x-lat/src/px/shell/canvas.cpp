// name: Gui.cpp
// type: c++
// desc: class implementation
// auth: is0urce

#include "canvas.hpp"

#include <px/common/string.hpp>

namespace px
{
	namespace shell
	{
		canvas::canvas(point2::component x, point2::component y)
			: matrix2<symbol>(x, y)
			, m_front(1, 1, 1, 1)
			, m_back(0, 0, 0, 0)
		{
		}
		canvas::~canvas()
		{
		}

		void canvas::cls(const symbol &clear_color)
		{
			fill(clear_color);
		}

		void canvas::cls()
		{
			fill({ ' ', m_front, m_back });
		}

		void canvas::text_color(color front_color)
		{
			m_front = front_color;
		}

		void canvas::draw_color(color back_color)
		{
			m_back = back_color;
		}

		void canvas::rectangle(const px::rectangle &bounds)
		{
			rectangle(bounds, m_back);
		}

		void canvas::rectangle(const px::rectangle &bounds, const color &back)
		{
			bounds.intersection({ { 0, 0 }, point2(width(), height()) }).enumerate([&](unsigned int i, unsigned int j)
			{
				symbol &s = at(i, j);
				s.code = ' ';
				s.back = back;
			});
		}

		void canvas::pset(const point2& pos)
		{
			pset(pos, m_back);
		}

		void canvas::pset(const point2& position, const color& back)
		{
			if (contains(position))
			{
				at(position).back = back;
			}
		}

		void canvas::write(const point2 &position, symbol::code_t code, const color &front)
		{
			if (contains(position))
			{
				auto &s = at(position);
				s.code = code;
				s.front = front;
			}
		}

		void canvas::write(const point2 &position, symbol::code_t code)
		{
			write(position, code, m_front);
		}

		void canvas::write(point2 position, const std::string &text, const color &front)
		{
			string::enum_utf8(text, [&](unsigned int letter)
			{
				write(position, letter, front);
				position.move(point2(1, 0));
			});
		}
		size_t canvas::write_n(point2 position, const std::string &text, const color &front, size_t max_symbols)
		{
			string::enum_utf8(text, [&](unsigned int letter)
			{
				if (max_symbols > 0)
				{
					write(position, letter, front);
					position.move(point2(1, 0));
					--max_symbols;
				}
			});
			return max_symbols;
		}

		void canvas::write(point2 position, const std::string &text)
		{
			write(position, text, m_front);
		}
	}
}