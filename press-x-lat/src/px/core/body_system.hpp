// name: body_system.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_CORE_BODY_SYSTEM_HPP
#define PX_CORE_BODY_SYSTEM_HPP

#include <px/es/i_system.hpp>
#include <px/es/manager.hpp>

#include "body_component.hpp"

namespace px
{
	namespace core
	{
		class body_system
			: public es::i_system
			, public es::manager<body_component, 10000>
		{
		public:
			body_system() {}
			virtual ~body_system() {}
		};
	}
}

#endif