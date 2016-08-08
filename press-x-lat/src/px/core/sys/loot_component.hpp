// name: loot_component.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_CORE_CONTAINER_COMPONENT_HPP
#define PX_CORE_CONTAINER_COMPONENT_HPP

#include <px/es/i_component.hpp>
#include <px/core/sys/location_component.hpp>
#include <px/core/sys/body_component.hpp>
#include <px/core/sys/i_useable_component.hpp>

namespace px
{
	namespace core
	{
		class environment;
		class loot_component
			: public es::i_component
			, public i_useable_component
			, public es::component_link<body_component>
		{
		public:
			loot_component() {}
			virtual ~loot_component() {}

		protected:
			virtual bool useable_component(location_component* user_pawn, environment& env) const override { return true; }
			virtual void use_component(location_component* user_pawn, environment& env) override {}
		};
	}
}

#endif