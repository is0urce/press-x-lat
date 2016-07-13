// name: persistency.hpp
// type: c++ header
// desc: enum
// auth: is0urce

#ifndef PX_CORE_PERSISTENCY_HPP
#define PX_CORE_PERSISTENCY_HPP

namespace px
{
	namespace core
	{
		enum class persistency : unsigned int
		{
			serialized,
			destroying,
			permanent,
			temporary
		};
	}
}

#endif