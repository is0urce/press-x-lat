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
		class biome
		{
			rl::tile<image> ground;

			std::list<image> trees;
			double trees_density;
			double flora_density;

			double predator_density;
			double prey_density;

			double rock_density;
			double vein_density;
		};
	}
}

#endif