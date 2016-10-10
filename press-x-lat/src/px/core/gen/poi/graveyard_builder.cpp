// name: graveyard_builder.cpp
// type: c++
// desc: class methods implementation
// auth: is0urce

#include "graveyard_builder.hpp"

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
			static const unsigned int sector_size = 15;
			static const int margin_size = 2;
			typedef std::mt19937 rng_type;
		}

		graveyard_builder::~graveyard_builder() {}

		void graveyard_builder::run_builder(unsigned int seed, rectangle const& bounds, fn::build_result &result) const
		{
			rng_type rng(seed);
			rng.discard(rng_type::state_size);

			fn::bsp<> sectors(rng, bounds, 20, 1);

			sectors.enumerate_bounds([&](rectangle sector) {

				if (std::uniform_int_distribution<int>(0, 1)(rng) == 0)
				{
					fn::build_fence(sector, fn::build_tile::wall, result, rng);
					sector.deflate(margin_size);
				}

				sector.enumerate([&](point2 const& location) {
					result.tiles[location] = fn::build_tile::no_change;
				});

				point2 tomb_align = fn::random_block_direction(rng);
				int mod_x = std::uniform_int_distribution<int>{ 3, 5 }(rng); // size of grave cell
				int mod_y = mod_x;
				int off_x = std::uniform_int_distribution<int>{ 0, mod_x - 1 }(rng);
				int off_y = std::uniform_int_distribution<int>{ 0, mod_y - 1 }(rng);

				sector.enumerate([&](point2 const& location) {
					auto d = location - sector.start();
					if (d.x() % mod_x == off_x && d.y() % mod_y == off_y)
					{
						result.tiles[location] = fn::build_tile::gravel;
						result.tiles[location.moved(tomb_align)] = fn::build_tile::doodad_tombstone;
					}
				});
			});
		}
	}
}