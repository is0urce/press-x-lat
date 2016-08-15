// name: world.hpp
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_CORE_WORLD_HPP
#define PX_CORE_WORLD_HPP

#include <px/common/matrix.hpp>

#include <px/core/gen/world_cell.hpp>
#include <px/core/gen/river_net.hpp>
#include <px/core/gen/city.hpp>

#include <list>
#include <random>

namespace px
{
	namespace core
	{
		class world
		{
		public:
			typedef world_cell cell_type;
			typedef matrix2<cell_type> map_type;
			typedef std::mt19937 rng_type;

		public:
			void resize(size_t width, size_t height);
			void clear();

			void generate(unsigned int seed);
			void generate_landmass();
			void generate_climate(double rivers);
			void generate_civilisation(unsigned int cities);
			void generate_appearance();

			map_type* map();
			const map_type* map() const;

		public:
			world();
			virtual ~world();
			world(const world&) = delete;

		private:
			void clear_cell(cell_type &cell);
			void expand_moisture();
			void generate_river(int x, int y, double size, river&);

		private:
			unsigned int m_seed; // world unique seed
			rng_type m_generator; // rng state

			map_type m_map;
			cell_type m_outer; // out-of-border cell props

			std::list<river> m_rivers;
			std::list<city> m_cities;
		};
	}
}

#endif