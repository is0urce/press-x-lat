// name: world_cell.hpp
// type: c++ header
// desc: struct declaration
// auth: is0urce

// proprities container of world map cell

#ifndef PX_CORE_WORLD_CELL_HPP
#define PX_CORE_WORLD_CELL_HPP

#include <px/common/vector.hpp>
#include <px/core/image.hpp>
#include <px/core/gen/river.hpp>

namespace px
{
	namespace core
	{
		struct world_cell
		{
			double altitude;
			double temperature;
			double moisture;
			river* river;
			double river_size;

			vector2 gradient; // dx, dy of altitude

			image img;
			unsigned int seed;
			bool generated;
		};
	}
}

#endif