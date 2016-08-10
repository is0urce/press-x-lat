// name: world.hpp
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_CORE_WORLD_GENERATOR_HPP
#define PX_CORE_WORLD_GENERATOR_HPP

#include <px/core/world_cell.hpp>
#include <px/common/matrix.hpp>
#include <px/common/vector.hpp>

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
			static const unsigned int perlin_width = 10;
			static const unsigned int perlin_height = perlin_width;
			static const unsigned int perlin_samples = 25;

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

				double w = m_map->width();
				double h = m_map->height();
				double mx = (perlin_width - 1) / w;
				double my = (perlin_height - 1) / h;

				vector2 center(w / 2.0f, h / 2.0f);
				double size = (std::min)(w, h);

				double max_peak = -1.0;
				m_map->enumerate([&](int i, int j, auto& cell)
				{
					double noise_magnitude = noise.sample(mx * i, my * j, perlin_samples);
					double distance_to_center = center.distance(vector2(i, j));
					double cone_magnitude = (size - distance_to_center * 4) / size; // [-1..1]

					cell.altitude = std::atan(cone_magnitude) + noise_magnitude;

					max_peak = (std::max)(max_peak, cell.altitude);
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
					cell.img.glyph = '~';
					cell.img.tint = { 1, 1, 1 };
					cell.img.bg = { 0, 0, 1 };

					if (cell.altitude > 0)
					{
						cell.img.glyph = '"';
						cell.img.tint = { 0, 0, 0 };
						cell.img.bg = { 0, 0.5, 0 };
					}

					if (cell.altitude > 0.6)
					{
						cell.img.glyph = '^';
						cell.img.tint = { 0, 0, 0 };
						cell.img.bg = { 0.5, 0.5, 0.5 };
					}
					if (cell.altitude > 0.8)
					{
						cell.img.glyph = '^';
						cell.img.tint = { 0.5, 0.5, 1 };
						cell.img.bg = { 1, 1, 1 };
					}
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