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
		class location_component;
		class factory;
		struct unit_record;

		class terrain_director
		{
		public:
			typedef matrix2<rl::tile<image>, settings::cell_width, settings::cell_height> map_type;
			typedef std::list<unit_record> unit_list;

		public:
			void generate(unsigned int seed);
			void generate_cell(point2 const& cell, map_type &terrain, unit_list &units);
			void generate_cell(point2 const& cell, map_type &terrain, unit_list &units, bool static_mobiles);
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