// name: builder.hpp
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_CORE_GEN_BUILDER_HPP
#define PX_CORE_GEN_BUILDER_HPP

#include <px/common/rectangle.hpp>

#include <px/fn/build_result.hpp>

namespace px
{
	namespace core
	{
		class builder
		{
		public:
			virtual void run(unsigned int seed, rectangle const& bounds, fn::build_result &result) const
			{
				run_builder(seed, bounds, result);
			}

		public:
			virtual void run_builder(unsigned int seed, rectangle const& bounds, fn::build_result &result) const = 0;

		public:
			virtual ~builder() = 0 {}
		};
	}
}

#endif