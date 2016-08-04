// name: i_useable_component.hpp
// type: c++ header
// desc: interface template class declaration
// auth: is0urce

#ifndef PX_CORE_I_USEABLE_COMPONENT_HPP
#define PX_CORE_I_USEABLE_COMPONENT_HPP

namespace px
{
	namespace core
	{
		class location_component;
		class environment;
		class i_useable_component
		{
		public:
			virtual ~i_useable_component() {}

		protected:
			virtual void use_component(location_component* l, environment& env)
			{
			}
			virtual bool useable_component(location_component* l, environment& env) const
			{
				return false;
			}

		public:
			bool useable(location_component* user, environment& env) const { return useable_component(user, env); }
			void use(location_component* user, environment& env) { use_component(user, env); }

			bool try_use(location_component* user, environment& env)
			{
				if (useable_component(user, env))
				{
					use_component(user, env);
					return true;
				}
				return false;
			}
		};
	}
}

#endif