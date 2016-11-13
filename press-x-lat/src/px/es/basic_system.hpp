// name: basic_system.hpp
// type: c++ header
// desc: base class
// auth: is0urce

#pragma once

// base class for systems in component / enetity system composition

#include "quant.hpp"

namespace px
{
	namespace es
	{
		class basic_system
		{
		protected:
			virtual void update_system(quant const& /* time */) {}
			virtual void pre_update_system(quant const& /* time */) {}
			virtual void post_update_system(quant const& /* time */) {}
			virtual void fixed_update_system(quant const& /* time */) {}

		public:
			void update(quant const& time) { update_system(time); }
			void pre_update(quant const& time) { pre_update_system(time); }
			void post_update(quant const& time) { post_update_system(time); }
			void fixed_update(quant const& time) { fixed_update_system(time); }
		};
	}
}