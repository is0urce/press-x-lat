// name: world.hpp
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_CORE_WORLD_H
#define PX_CORE_WORLD_H

#include <px/rl/tile.hpp>
#include <px/rl/traverse.hpp>
#include <px/common/matrix.hpp>
#include <px/fn/perlin.hpp>

#include "location_component.hpp"
#include "unit.hpp"
#include "image.hpp"

#include <memory>
#include <list>
#include <random>
#include <cstdint>

namespace px
{
	namespace core
	{
		class world
		{
		public:
			static const unsigned int cell_width = 50;
			static const unsigned int cell_height = cell_width;
			static const unsigned int perlin_width = 5;
			static const unsigned int perlin_height = perlin_width;
			static const unsigned int perlin_samples = 9;

		public:
			typedef std::shared_ptr<unit> unit_ptr;
			typedef rl::tile<image> tile;
			typedef matrix2<tile, cell_width, cell_height> map;
			typedef std::mersenne_twister_engine<std::uint_fast32_t, 32, 624, 397, 31,
				0x9908b0df, 11,
				0xffffffff, 7,
				0x9d2c5680, 15,
				0xefc60000, 18, 1812433253> rng;

		private:
			unsigned int m_seed;
			matrix2<bool, cell_width, cell_height> m_created;

		public:
			world(unsigned int seed) : m_seed(seed)
			{
				generate(seed);
			}
			virtual ~world()
			{
			}
			world(const world&) = delete;

		public:
			void generate(unsigned int seed)
			{
				m_created.fill(false);
			}

			// management
			void arrange(const point2 &cell, map& terrain, std::list<unit_ptr>& units)
			{
				rng generator(m_seed + cell.x() + cell.y() * cell_width * cell_height);
				std::uniform_real_distribution<double> distribution;
				fn::perlin<perlin_width, perlin_height> noise([&](){return distribution(generator); });

				double mx = static_cast<double>(perlin_width) / cell_width;
				double my = static_cast<double>(perlin_height) / cell_height;

				terrain.enumerate([&](int i, int j, tile& t)
				{
					auto magnitude = noise.sample(mx * i, my * j, perlin_samples);
					auto &img = t.appearance();
					if (magnitude > 0.0)
					{
						img.glyph = '.';
						img.tint = { 0, 1, 0 };
						t.make_traversable();
					}
					else
					{
						img.glyph = '#';
						img.tint = { 0.5, 0.5, 0.5 };
						t.make_blocking();
					}
				});
			}
			void store(unit_ptr unit)
			{

			}
		};
	}
}

#endif