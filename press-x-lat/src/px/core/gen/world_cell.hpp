// name: world_cell.hpp
// type: c++ header
// desc: struct declaration
// auth: is0urce

#ifndef PX_CORE_WORLD_CELL_H
#define PX_CORE_WORLD_CELL_H

#include <px/core/image.hpp>

namespace px
{
	namespace core
	{
		struct world_cell
		{
		public:
			double altitude;
			bool generated;
			image img;

			world_cell() : generated(false)
			{

			}
		};
	}
}

#endif