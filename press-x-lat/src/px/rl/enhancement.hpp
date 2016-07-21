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
		template<typename _E>
		struct enhancement
		{
			_E type;
			unsigned int subtype;

			int value;
			int bonus;
			double magnitude;
			double multiplier;
			double chance;
			float flag;
		};
	}
}

#endif