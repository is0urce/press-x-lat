// name: world_cell.hpp
// type: c++ header
// desc: struct declaration
// auth: is0urce

// proprities container of world map cell

#ifndef PX_CORE_WORLD_CELL_HPP
#define PX_CORE_WORLD_CELL_HPP

#include <px/core/image.hpp>

namespace px
{
	namespace core
	{
		struct world_cell
		{
			double altitude;
			double temperature;
			double moisture;
			double river;

			image img;
			unsigned int seed;
			bool generated;
		};
	}
}

#endif