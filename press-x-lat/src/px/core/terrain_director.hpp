// name: terrain_builder.hpp
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_CORE_TERRAIN_DIRECTOR_HPP
#define PX_CORE_TERRAIN_DIRECTOR_HPP

#include <px/common/matrix.hpp>

#include <px/rl/tile.hpp>
#include <px/rl/traverse.hpp>

#include <px/core/gen/world_cell.hpp>
#include <px/core/gen/world.hpp>

#include <px/core/settings.hpp>
#include <px/core/image.hpp>

#include <memory>
#include <list>

namespace px
{
	namespace core
	{
		class unit;
		class factory;

		class terrain_director
		{
		public:
			static const unsigned int cell_width = settings::cell_width;
			static const unsigned int cell_height = settings::cell_height;

		public:
			typedef std::shared_ptr<unit> unit_ptr;
			typedef rl::tile<image> tile_type;
			typedef matrix2<tile_type, cell_width, cell_height> map_type;

		public:
			void generate(unsigned int seed);
			void generate_cell(const point2 &cell, map_type& terrain, std::list<unit_ptr>& units);
			void generate_cell(const point2 &cell, map_type& terrain, bool static_mobiles, std::list<unit_ptr>& units);
			world::map_type* map();
			const world::map_type* map() const;

		public:
			terrain_director(world &w, factory &factory);
			virtual ~terrain_director();
			terrain_director(const terrain_director&) = delete;

		private:
			unsigned int m_seed;
			factory* m_factory;
			world* m_world;
			world_cell m_outer;
		};
	}
}

#endif