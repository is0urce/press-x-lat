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

namespace px
{
	namespace core
	{
		class world
		{
		public:
			static const unsigned int cell_width = 10;
			static const unsigned int cell_height = 10;

		public:
			typedef std::shared_ptr<unit> unit_ptr;
			typedef rl::tile<image> tile;
			typedef matrix2<tile, cell_width, cell_height> map;

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
				fn::perlin<2, 2> noise;
				terrain.enumerate([&](int i, int j, tile& t)
				{
					auto magnitude = noise.sample(i, j, 3);
					if (magnitude > -0.3)
					{
						t.appearance().glyph = '.';
						t.appearance().tint = { 1, 1, 1 };
						t.make_traversable();
					}
					else
					{
						t.appearance().glyph = '#';
						t.appearance().tint = { 0.5, 0.5, 0.5 };
						t.make_blocking();
					}

				});
			}
			void store(unit_ptr unit)
			{

			}

			// serialization

		};
	}
}

#endif