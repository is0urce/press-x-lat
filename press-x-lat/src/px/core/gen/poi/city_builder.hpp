// name: city_builder.hpp
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_CORE_GEN_POI_CITY_BUILDER_HPP
#define PX_CORE_GEN_POI_CITY_BUILDER_HPP

#include <px/core/gen/builder.hpp>

namespace px
{
	namespace core
	{
		class city_builder : public builder
		{
		public:
			city_builder() = default;
			virtual ~city_builder() = default;

		public:
			virtual void run_builder(unsigned int seed, rectangle const& bounds, fn::build_result &result) const override
			{
				result.placeables.push_back({ 0, 0 }, fn::build_placeable::workshop_anvil);
			}
		};
	}
}

#endif