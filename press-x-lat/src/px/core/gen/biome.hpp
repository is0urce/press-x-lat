// name: river.hpp
// type: c++ header
// desc: struct declaration
// auth: is0urce

#ifndef PX_CORE_BIOME_HPP
#define PX_CORE_BIOME_HPP

#include <px/common/point.hpp>
#include <px/common/color.hpp>

#include <px/core/image.hpp>
#include <px/rl/tile.hpp>

#include <list>

namespace px
{
	namespace core
	{
		struct biome
		{
			rl::tile<image> ground; // ground props
			image appearance; // on the global map

			double flora_density;
			double trees_density;
			std::list<image> trees;

			double predator_density;
			double prey_density;

			double rock_density;
			double vein_density;

			void clear()
			{
				ground.make_transparent();
				ground.make_traversable();

				trees_density = 0;
				flora_density = 0;
				trees.clear();

				predator_density = 0;
				prey_density = 0;

				rock_density = 0;
				vein_density = 0;
			}
		};
	}
}

#endif