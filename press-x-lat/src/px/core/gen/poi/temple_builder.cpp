// name: temple_builder.cpp
// type: c++
// desc: class methods implementation
// auth: is0urce

#include "temple_builder.hpp"

#include <px/common/rectangle.hpp>
#include <px/fn/bsp.hpp>
#include <px/fn/build_housing.hpp>
#include <px/fn/build_aux.hpp>

#include <px/core/gen/builder.hpp>

#include <random>

namespace px
{
	namespace core
	{
		namespace
		{
			typedef std::mt19937 rng_type;
		}

		temple_builder::~temple_builder() {}

		void temple_builder::run_builder(unsigned int seed, rectangle const& bounds, fn::build_result &result) const
		{
			rng_type rng(seed);
			rng.discard(rng_type::state_size);
		}
	}
}