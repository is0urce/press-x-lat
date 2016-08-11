// name: world.cpp
// type: c++ header
// desc: class methods implementation
// auth: is0urce

#include "world.hpp"

#include <px/common/vector.hpp>

#include <px/core/gen/world_cell.hpp>
#include <px/fn/perlin.hpp>

#include <random>
#include <numeric>

namespace px
{
	namespace core
	{
		namespace
		{
			typedef std::mt19937 rng;
			static const unsigned int perlin_width = 10;
			static const unsigned int perlin_height = perlin_width;
			static const unsigned int perlin_samples = 25;
		}

		world::world()
		{
			m_outer.altitude = -1;
			m_outer.moisture = 0;
		}
		world::~world() {}

		void world::resize(unsigned int width, unsigned int height)
		{
			m_map.resize(width, height);
			m_rivers.resize(width + 1, height + 1);
		}
		void world::generate(unsigned int seed)
		{
			m_seed = seed;
			clear();
			generate_landmass();
			generate_appearance();
		}
		void world::clear()
		{
			m_map.fill(m_outer);
			m_rivers.clear();
		}

		// generate altitudes [-1+..1], sea level is 0
		void world::generate_landmass()
		{
			std::array<unsigned int, 624> seed_data;
			std::iota(std::begin(seed_data), std::end(seed_data), m_seed);
			std::seed_seq state(std::begin(seed_data), std::end(seed_data));

			rng generator(state);
			generator.discard(rng::state_size);
			fn::perlin<perlin_width, perlin_height> noise([generator, distribution = std::uniform_real_distribution<double>()]() mutable {return distribution(generator); });

			double w = m_map.width();
			double h = m_map.height();
			double mx = (perlin_width - 1) / w;
			double my = (perlin_height - 1) / h;

			vector2 center(w / 2.0f, h / 2.0f);
			double size = (std::min)(w, h);

			double max_peak = -1.0;
			m_map.enumerate([&](int i, int j, auto& cell)
			{
				double noise_magnitude = noise.sample(mx * i, my * j, perlin_samples);
				double distance_to_center = center.distance(vector2(i, j));
				double cone_magnitude = (size - distance_to_center * 4) / size; // [-1..1]

				cell.altitude = std::atan(cone_magnitude) + noise_magnitude;

				max_peak = (std::max)(max_peak, cell.altitude);
			});

			// normalise
			m_map.enumerate([mx, my, max_peak](int i, int j, auto& cell)
			{
				cell.altitude = cell.altitude / max_peak;
			});
		}

		void world::generate_appearance()
		{
			m_map.enumerate([](int i, int j, auto& cell)
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

				if (cell.altitude > 0.5)
				{
					cell.img.glyph = '^';
					cell.img.tint = { 0, 0, 0 };
					cell.img.bg = { 0.5, 0.5, 0.5 };
				}
				if (cell.altitude > 0.75)
				{
					cell.img.glyph = '^';
					cell.img.tint = { 0.5, 0.5, 1 };
					cell.img.bg = { 1, 1, 1 };
				}
			});
		}

		world::map_type* world::map()
		{
			return &m_map;
		}
		const world::map_type* world::map() const
		{
			return &m_map;
		}
	}
}