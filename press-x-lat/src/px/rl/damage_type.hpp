// name: damage_type.hpp
// type: c++ header
// desc: enum
// auth: is0urce

#ifndef PX_RL_DAMAGE_TYPE_HPP
#define PX_RL_DAMAGE_TYPE_HPP

namespace px
{
	namespace rl
	{
		enum class damage_type : unsigned int
		{
			pure,

			piercing,
			slashing,
			blunt,
			gravitation,

			arcane,
			curse,
			anomaly,
			disintegration,

			inexplicable,
			sonic,
			magnetic,
			mental,

			ice,
			acid,
			shock,
			fire,

			light,
			poison,
			disease,
			darkness,

			not_valid,
			min_value = pure,
			max_value = not_valid
		};
	}
}

#endif