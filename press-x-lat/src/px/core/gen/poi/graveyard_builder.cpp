// name: graveyard_builder.cpp
// type: c++
// desc: class methods implementation
// auth: is0urce

#include "graveyard_builder.hpp"

#include <px/common/rectangle.hpp>
#include <px/fn/bsp.hpp>

#include <px/core/gen/build_result.hpp>
#include <px/core/gen/builder.hpp>

#include <random>

namespace px
{
	namespace core
	{
		namespace
		{
			static const unsigned int sector_size = 15;
			static const unsigned int room_size = 4;
			typedef std::mt19937 rng_type;

			rng_type generator(unsigned int seed)
			{
				return rng_type(seed);
			}
		}

		graveyard_builder::~graveyard_builder() {}

		void graveyard_builder::run_builder(unsigned int seed, rectangle const& bounds, build_result &result) const
		{
			rng_type rng(seed);
			rng.discard(rng_type::state_size);

			bounds.enumerate([&](point2 const& location) {
				result.tiles[location] = build_tile::no_change;
			});

			bounds.enumerate([&](point2 const& location) {
				if (location.x() % 4 == 0 && location.y() % 4 == 0)
				{
					result.tiles[location] = build_tile::background_doodad;
				}
			});

		}
	}
}