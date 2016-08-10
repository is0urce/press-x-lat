// name: world.hpp
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_CORE_WORLD_HPP
#define PX_CORE_WORLD_HPP

#include <px/common/matrix.hpp>

#include <px/core/world_cell.hpp>
#include <px/core/image.hpp>

#include <px/rl/tile.hpp>
#include <px/rl/traverse.hpp>

#include <memory>
#include <list>

namespace px
{
	namespace core
	{
		class unit;
		class factory;
		class world
		{
		public:
			static const unsigned int world_width = 10;
			static const unsigned int world_height = world_width;
			static const unsigned int cell_width = 100;
			static const unsigned int cell_height = cell_width;

		public:
			typedef std::shared_ptr<unit> unit_ptr;
			typedef rl::tile<image> tile_type;
			typedef matrix2<tile_type, cell_width, cell_height> local_map_type;
			typedef matrix2<world_cell> world_map_type;

		public:
			void generate(unsigned int seed);
			void arrange(const point2 &cell, local_map_type& terrain, std::list<unit_ptr>& units);
			void generate(const point2 &cell, local_map_type& terrain, bool static_mobiles, std::list<unit_ptr>& units);
			void store(unit_ptr unit);

		public:
			world(factory &factory);
			virtual ~world();
			world(const world&) = delete;

		private:
			unsigned int m_seed;
			factory* m_factory;
			world_map_type m_map;
			world_cell m_outer;
		};
	}
}

#endif