// name: builder.hpp
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_CORE_GEN_BUILDER_HPP
#define PX_CORE_GEN_BUILDER_HPP

#include <px/common/rectangle.hpp>

namespace px
{
	namespace core
	{
		struct build_result;

		class builder
		{
		public:
			virtual void build(unsigned int seed, rectangle const& bounds, build_result &result) const = 0;
			virtual ~builder() = 0 {}
		};
	}
}

#endif