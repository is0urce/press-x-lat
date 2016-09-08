// name: world_aspect.hpp
// type: c++ header
// desc: enum definition
// auth: is0urce

#ifndef PX_CORE_WORLD_ASPECT_HPP
#define PX_CORE_WORLD_ASPECT_HPP

namespace px
{
	namespace core
	{
		enum class world_aspect : unsigned int
		{
			inertia = 0,
			knowledge = 1,
			sentience = 2,
			entropy = 3,
			existence = 4,

			max_value = existence
		};
	}
}

#endif