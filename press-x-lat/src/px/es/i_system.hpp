// name: i_system.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_ES_I_SYSTEM_HPP
#define PX_ES_I_SYSTEM_HPP

namespace px
{
	namespace es
	{
		class i_system
		{
		public:
			virtual ~i_system() = 0 {}

		protected:
			virtual void update_system() {}
			virtual void pre_update_system() {}
			virtual void post_update_system() {}
			virtual void fixed_update_system() {}

		public:
			void update() { update_system(); }
			void pre_update() { pre_update_system(); }
			void post_update() { post_update_system(); }
			void fixed_update() { fixed_update_system(); }
		};
	}
}

#endif