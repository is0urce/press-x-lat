// name: farm_builder.hpp
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_CORE_GEN_POI_FARM_BUILDER_HPP
#define PX_CORE_GEN_POI_FARM_BUILDER_HPP

#include <px/core/gen/builder.hpp>

namespace px
{
	namespace core
	{
		class farm_builder : public builder
		{
		public:
			farm_builder() = default;
			virtual ~farm_builder();

		public:
			virtual void run_builder(unsigned int seed, rectangle const& bounds, fn::build_result &result) const override;
		};
	}
}

#endif