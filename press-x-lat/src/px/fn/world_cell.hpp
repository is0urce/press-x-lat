// name: world_cell.hpp
// type: c++ header
// desc: struct declaration
// auth: is0urce

// proprities container of world map cell

#ifndef PX_FN_WORLD_CELL_HPP
#define PX_FN_WORLD_CELL_HPP

#include <px/core/image.hpp>

namespace px
{
	namespace fn
	{
		struct world_cell
		{
			double altitude;
			double temperature;
			double moisture;
			double river;

			core::image img;
			unsigned int seed;
			bool generated;
		};
	}
}

#endif