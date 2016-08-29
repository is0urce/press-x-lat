// name: graveyard_builder.hpp
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_CORE_GEN_POI_GRAVEYARD_BUILDER_HPP
#define PX_CORE_GEN_POI_GRAVEYARD_BUILDER_HPP

#include <px/core/gen/builder.hpp>

namespace px
{
	namespace core
	{
		class graveyard_builder : public builder
		{
		public:
			graveyard_builder() = default;
			virtual ~graveyard_builder();

		public:
			virtual void run_builder(unsigned int seed, rectangle const& bounds, build_result &result) const override;
		};
	}
}

#endif