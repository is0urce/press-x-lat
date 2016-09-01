// name: farm_builder.cpp
// type: c++
// desc: class methods implementation
// auth: is0urce

#include "farm_builder.hpp"

#include <px/common/rectangle.hpp>

#include <px/fn/bsp.hpp>
#include <px/fn/build_result.hpp>
#include <px/fn/build_housing.hpp>

#include <px/fn/build_aux.hpp>

#include <px/core/gen/builder.hpp>

#include <random>
#include <tuple>

namespace px
{
	namespace core
	{
		namespace
		{
			static const unsigned int sector_size = 15;
			static const unsigned int room_size = 4;
			typedef std::mt19937 rng_type;

			enum class farm_sector : int
			{
				house,
				field,
				barn,
				empty,
				garden,

				min_value = house,
				max_value = garden
			};
		}

		farm_builder::~farm_builder() {}

		void farm_builder::run_builder(unsigned int seed, rectangle const& bounds, fn::build_result &result) const
		{
			rng_type rng(seed);
			rng.discard(rng_type::state_size);

			bounds.enumerate([&](point2 const& location) {
				result.tiles[location] = fn::build_tile::no_change;
			});

			fn::bsp<rng_type>(rng, bounds, sector_size).enumerate([&](auto const& sector) {
				auto sector_bounds = sector.bounds.deflated(1); // some space between "districts"
				switch (fn::random_enum<farm_sector>(rng))
				{
				case farm_sector::house:
					build_house(room_size, fn::random_rectangle<4>(sector_bounds, rng), result, rng);
					break;
				case farm_sector::barn:
					build_barn(fn::random_rectangle<4>(sector_bounds, rng), result, rng);
					break;
				case farm_sector::field:
					build_field(sector_bounds, result, rng);
					break;
				case farm_sector::empty: // no change, keep background trees
				case farm_sector::garden: 
					break;
				}
			});
		}
	}
}