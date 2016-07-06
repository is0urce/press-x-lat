// name: opengl.cpp
// type: c++
// desc: class implementation
// auth: is0urce

#include "opengl.h"

namespace px
{
	namespace shell
	{
		opengl::opengl()
		{
		}

		opengl::~opengl()
		{
		}

		void opengl::update(int &screen_width, int &screen_height)
		{
			update_screen(screen_width, screen_height);
		}

		void opengl::swap()
		{
			swap_buffers();
		}
	}
}