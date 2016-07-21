// name: body_component.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_CORE_BODY_COMPONENT_HPP
#define PX_CORE_BODY_COMPONENT_HPP

#include <px/es/i_component.hpp>
#include <px/es/component_link.hpp>
#include <px/rl/body.hpp>
#include <px/rl/character.hpp>

namespace px
{
	namespace core
	{
		class body_component
			: public es::i_component
			, public rl::body
			, public rl::character<body_component&>
		{
		public:
			virtual ~body_component() {}
		};
	}
}

#endif