// name: graveyard_builder.cpp
// type: c++
// desc: class methods implementation
// auth: is0urce

#include "graveyard_builder.hpp"

#include <px/common/rectangle.hpp>
#include <px/fn/bsp.hpp>
#include <px/fn/build_housing.hpp>

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

		void graveyard_builder::run_builder(unsigned int seed, rectangle const& bounds, fn::build_result &result) const
		{
			rng_type rng(seed);
			rng.discard(rng_type::state_size);

			fn::bsp<> sectors(rng, bounds, 20, 1);

			sectors.enumerate_bounds([&](rectangle sector) {

				if (std::uniform_int_distribution<int>(0, 1)(rng) == 0)
				{
					fn::build_fence(sector, fn::build_tile::wall, result, rng);
					sector.deflate(2);
				}

				sector.enumerate([&](point2 const& location) {
					result.tiles[location] = fn::build_tile::no_change;
				});

				point2 start = sector.start();
				sector.enumerate([&](point2 const& location) {
					auto d = location - start;
					int dx = d.x() % 4;
					int dy = d.y() % 4;
					if (dx == 1)
					{
						if (dy == 2)
						{
							result.tiles[location] = fn::build_tile::doodad_tombstone;
						}
						else if (dy == 1)
						{
							result.tiles[location] = fn::build_tile::gravel;
						}
					}
				});
			});
		}
	}
}