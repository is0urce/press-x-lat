// name: world.hpp
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_CORE_WORLD_GENERATOR_HPP
#define PX_CORE_WORLD_GENERATOR_HPP

#include <px/core/world_cell.hpp>
#include <px/common/matrix.hpp>

#include <px/fn/perlin.hpp>

#include <random>
#include <numeric>

namespace px
{
	namespace core
	{
		class world_generator
		{
		public:
			typedef matrix2<world_cell> map_type;
			typedef std::mt19937 rng;
			static const unsigned int perlin_width = 5;
			static const unsigned int perlin_height = perlin_width;
			static const unsigned int perlin_samples = 9;

		public:
			void generate()
			{
				generate_landmass();
				generate_appearance();
			}

			// generate altitudes [-1+..1], sea level is 0
			void generate_landmass()
			{
				std::array<unsigned int, 624> seed_data;
				std::iota(std::begin(seed_data), std::end(seed_data), m_seed);
				std::seed_seq state(std::begin(seed_data), std::end(seed_data));

				rng generator(state);
				generator.discard(rng::state_size);
				fn::perlin<perlin_width, perlin_height> noise([generator, distribution = std::uniform_real_distribution<double>()]() mutable {return distribution(generator); });

				double mx = static_cast<double>(perlin_width - 1) / m_map->width();
				double my = static_cast<double>(perlin_height - 1) / m_map->height();

				double max_peak = -1.0;
				m_map->enumerate([mx, my, &noise, &max_peak](int i, int j, auto& cell)
				{
					double altitude = noise.sample(mx * i, my * j, perlin_samples);

					cell.altitude = altitude;

					if (max_peak < altitude) max_peak = altitude;
				});

				// normalise
				m_map->enumerate([mx, my, max_peak](int i, int j, auto& cell)
				{
					cell.altitude = cell.altitude / max_peak;
				});
			}

			void generate_appearance()
			{
				m_map->enumerate([](int i, int j, auto& cell)
				{
					cell.img.glyph = cell.altitude > 0 ? '^' : '.';
				});
			}

		public:
			world_generator(map_type &map, unsigned int seed) : m_seed(seed), m_map(&map) {}
			virtual ~world_generator() {}
			world_generator(const world_generator&) = delete;

		private:
			unsigned int m_seed;
			map_type* m_map;
		};
	}
}

#endif