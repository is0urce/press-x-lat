// name: container_component.cpp
// type: c++ header
// desc: class methods implementation
// auth: is0urce

#include "container_component.hpp"

#include <px/core/environment.hpp>

namespace px
{
	namespace core
	{
		container_component::container_component() {}
		container_component::~container_component() {}

		bool container_component::useable_component(location_component* user_pawn, environment& env) const
		{
			return true;
		}
		void container_component::use_component(location_component* user_pawn, environment& env)
		{
			env.open_container(user_pawn->linked(), linked());
		}
	}
}