// name: world.hpp
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_CORE_WORLD_HPP
#define PX_CORE_WORLD_HPP

#include <px/common/matrix.hpp>

#include <px/core/gen/world_cell.hpp>
#include <px/core/gen/river_net.hpp>

namespace px
{
	namespace core
	{
		class world
		{
		public:
			typedef world_cell cell_type;
			typedef matrix2<cell_type> map_type;

		public:
			void resize(unsigned int width, unsigned int height);
			void clear();
			void generate(unsigned int seed);
			void generate_landmass();
			void generate_appearance();
			map_type* map();
			const map_type* map() const;

		public:
			world();
			virtual ~world();
			world(const world&) = delete;

		private:
			unsigned int m_seed;
			map_type m_map;
			river_net m_rivers;
			cell_type m_outer; // out-of-border cell props
		};
	}
}

#endif