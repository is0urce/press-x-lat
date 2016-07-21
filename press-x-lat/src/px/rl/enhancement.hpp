// name: enhancement.hpp
// type: c++ header
// desc: struct
// auth: is0urce

#ifndef PX_RL_ENHANCEMENT_HPP
#define PX_RL_ENHANCEMENT_HPP

namespace px
{
	namespace rl
	{
		struct enhancement
		{
			int magnitude;
			unsigned int type;
			double multiplier;
			double chance;
		};
	}
}

#endif