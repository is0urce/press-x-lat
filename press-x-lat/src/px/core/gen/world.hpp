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
		class factory;
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
			void generate_biomes();
			void generate_civilisation(unsigned int cities);
			void generate_appearance();

			map_type* map();
			const map_type* map() const;
			point2 spawn() const noexcept;
			cell_type& select(point2 const& location);
			cell_type const& select(point2 const& location) const;

		public:
			world(factory &unit_builder);
			virtual ~world();
			world(const world&) = delete;
			world& operator=(const world&) = delete;

		private:
			void clear_cell(cell_type &cell);
			void expand_moisture();
			void create_river(int x, int y, double size, river&);
			void assign_biome(cell_type &cell);

		private:
			unsigned int m_seed; // world unique seed
			rng_type m_generator; // rng state
			factory* m_factory;

			map_type m_map;
			cell_type m_outer; // out-of-border cell props

			biome m_ocean;
			biome m_mountain_rock;
			biome m_mountain_ice;
			biome m_plains;
			biome m_forest;
			biome m_tundra;
			biome m_taiga;
			biome m_desert;
			biome m_drylands;

			std::list<river> m_rivers;
			std::list<city> m_cities;

			point2 m_spawn;
		};
	}
}

#endif