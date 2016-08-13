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
			static const unsigned int perlin_width = 10;
			static const unsigned int perlin_height = perlin_width;
			static const unsigned int perlin_samples = 25;
			static const unsigned int max_try_stop = 10000;

			template <typename T, typename M>
			static T lerp(T a, T b, M w)
			{
				return a * (1 - w) + b * w;
			}

			template <typename T, typename M>
			static T lerp(T a, T b, M w, M w0, M w1)
			{
				M ww = (w - w0) / (w1 - w0);
				return ww < 0 ? a : ww > 1 ? b : lerp(a, b, (w - w0) / (w1 - w0));
			}
		}

		world::world()
		{
			// out-of-border cell proprieties
			m_outer.altitude = -1;
			m_outer.moisture = 0;
			m_outer.river = nullptr;
			m_outer.river_size = 0;
			m_outer.gradient = { 0, 0 };
		}
		world::~world() {}

		void world::resize(unsigned int width, unsigned int height)
		{
			m_map.resize(width, height);
			//m_rivers.resize(width + 1, height + 1);
		}
		void world::generate(unsigned int seed)
		{
			// setup and warmup random generator
			m_seed = seed;
			std::array<unsigned int, 624> seed_data;
			std::iota(std::begin(seed_data), std::end(seed_data), m_seed);
			std::seed_seq state(std::begin(seed_data), std::end(seed_data));
			m_generator.seed(state);
			m_generator.discard(rng::state_size);

			clear();
			generate_landmass();
			generate_climate(3);
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
			fn::perlin<perlin_width, perlin_height> noise(m_generator);

			double w = m_map.width();
			double h = m_map.height();
			double mx = (perlin_width - 1) / w;
			double my = (perlin_height - 1) / h;

			double max_peak = -1.0;

			// calculate altitudes
			vector2 center(w / 2.0f, h / 2.0f);
			double size = (std::min)(w, h);
			m_map.enumerate([&](int i, int j, auto& cell)
			{
				double noise_magnitude = noise.sample(mx * i, my * j, perlin_samples);
				double distance_to_center = center.distance(vector2(i, j));
				double cone_magnitude = (size - distance_to_center * 4) / size; // [-1..1]

				cell.altitude = std::atan(cone_magnitude) + noise_magnitude;

				max_peak = (std::max)(max_peak, cell.altitude);
			});

			// normalise, calculate gradient
			m_map.enumerate([mx, my, max_peak, this](int i, int j, auto& cell)
			{
				cell.altitude = cell.altitude / max_peak;

				auto dx = m_map.select({ i + 1, j }, m_outer).altitude - m_map.select({ i - 1, j }, m_outer).altitude;
				auto dy = m_map.select({ i, j + 1 }, m_outer).altitude - m_map.select({ i, j - 1 }, m_outer).altitude;

				cell.gradient = { dx, dy };
			});
		}

		void world::generate_climate(double rivers)
		{
			// fill moisture from ocean
			m_rivers.clear();
			m_map.enumerate([h = m_map.height()](unsigned int i, unsigned int j, auto& cell)
			{
				cell.moisture = cell.altitude <= 0 ? 1.0 : 0.0;
				cell.temperature = -0.25 + 1.0 * (h - j) / h - cell.altitude * 0.5;
				cell.river = nullptr;
			});
			expand_moisture();

			// generate rivers
			std::uniform_int_distribution<unsigned int> rx(0, m_map.width() - 1);
			std::uniform_int_distribution<unsigned int> ry(0, m_map.height() - 1);
			unsigned int n = 0;
			while (n < rivers)
			{
				unsigned int x = rx(m_generator);
				unsigned int y = ry(m_generator);

				auto &cell = m_map[point2(x, y)];
				if (cell.river_size <= 0 && cell.altitude > 0.35)
				{
					river r;
					r.set_tag(std::string("river") + std::to_string(n));

					m_rivers.emplace_back(r);
					generate_river(x, y, cell.moisture, m_rivers.back());
					++n;
				}
			}

			// fill moisture from rivers
			m_map.enumerate([](unsigned int i, unsigned int j, auto& cell)
			{
				cell.moisture = cell.river_size > 0 ? 1.0 : cell.altitude < 0 ? 0.0125 : 0.0;
			});
			expand_moisture();
		}

		void world::generate_appearance()
		{
			//if (temperature < 10.0)
			//{
			//	target = (moisture > 5) ? &taiga : &tundra;
			//}
			//else if (temperature < 25)
			//{
			//	target = (moisture > 5) ? &forest : &planes;
			//}
			//else
			//{
			//	target = (moisture > 5) ? &dryland : &desert;
			//}
			m_map.enumerate([w = m_map.width(), h = m_map.height(), this](int i, int j, auto& cell)
			{
				if (cell.altitude <= 0)
				{
					cell.img.glyph = '~';
					cell.img.tint = { 1, 1, 1 };
					cell.img.bg = color(0.25 + cell.altitude / 4.0, 0.25 + cell.altitude / 4.0, 1 + cell.altitude / 4.0); // -1,0 - > [0, 0.5]
				}
				else
				{
					if (cell.altitude < 0.5)
					{
						if (cell.temperature < 0)
						{
							cell.img.glyph = cell.moisture < 0.0625 ? '.' : 9650;
							cell.img.tint = cell.moisture < 0.0625 ? color(0, 0, 0) : color(0, 0.25, 0);
							cell.img.bg = lerp(color(0.9, 0.9, 0.9), color(1, 1, 1), cell.altitude, 0.0, 1.0);
						}
						else if (cell.temperature > 0.25 && cell.moisture < 0.0625)
						{
							cell.img.glyph = '.';
							cell.img.tint = { 0, 0, 0 };
							cell.img.bg = lerp(color(0.8, 0.8, 0.0), color(0.8, 0.8, 0.5), 1-cell.altitude, 0.5, 1.0);
						}
						else
						{
							cell.img.glyph = cell.moisture < 0.0625 ? '"' : 9650;
							cell.img.tint = { 0, 0.25, 0 };
							cell.img.bg = lerp(color(0.0, 0.5, 0.0), color(0, 0.75, 0), cell.altitude, 0.0, 1.0);
						}
					}
					else // mountains
					{
						cell.img.glyph = '^';
						if (cell.temperature > 0)
						{
							cell.img.tint = { 0, 0, 0 };
							cell.img.bg = { 0.5, 0.5, 0.5 };// lerp(color(0.5, 0.5, 0.5), color(0.55, 0.55, 0.55), cell.altitude, 0.5, 1.0);
						}
						else
						{
							cell.img.tint = lerp(color(0, 0, 0), color(0.75, 0.75, 1), cell.altitude, 0.5, 1.0);
							cell.img.bg = lerp(color(0.9, 0.9, 0.9), color(1, 1, 1), cell.altitude, 0.75, 1.0);
						}
					}

					if (cell.river_size > 0)
					{
						cell.img.glyph = '~';
						cell.img.tint = { 0, 0, 1 };
					}
				}
			});
		}

		void world::expand_moisture()
		{
			bool change = true;
			while (change)
			{
				change = false;
				m_map.enumerate([&change, this](int x, int y, auto& cell)
				{
					auto &u = m_map.select({ x, y + 1 }, m_outer);
					auto &d = m_map.select({ x, y - 1 }, m_outer);
					auto &l = m_map.select({ x - 1, y }, m_outer);
					auto &r = m_map.select({ x + 1, y }, m_outer);

					double moisture = (std::max)((std::max)(u.moisture, d.moisture), (std::max)(l.moisture, r.moisture)) / 2;

					if (cell.moisture < moisture)
					{
						cell.moisture = moisture;
						change = true;
					}
				});
			}
		}

		void world::generate_river(int x, int y, double size, river& current)
		{
			if (!m_map.contains(x, y)) return;

			auto &cell = m_map[{x, y}];

			size += cell.river_size; // join to another river

			cell.river = &current;
			cell.river_size = size;

			if (cell.altitude > 0)
			{
				auto &u = m_map.select({ x, y + 1 }, m_outer);
				auto &d = m_map.select({ x, y - 1 }, m_outer);
				auto &l = m_map.select({ x - 1, y }, m_outer);
				auto &r = m_map.select({ x + 1, y }, m_outer);

				int dx = 0;
				int dy = 0;
				std::uniform_int_distribution<unsigned int> even(0, 1);

				// flow down
				if (dx == 0 && dy == 0)
				{
					vector2 delta = cell.gradient;
					bool axis = std::abs(delta.x()) > std::abs(delta.y());
					if (std::abs(delta.x()) == std::abs(delta.y())) axis = even(m_generator) == 0;
					if (axis)
					{
						if (delta.x() < 0 && r.altitude < cell.altitude && r.river != &current)
						{
							++dx;
						}
						else if (delta.x() > 0 && l.altitude < cell.altitude && l.river != &current)
						{
							--dx;
						}
					}
					else
					{
						if (delta.y() < 0 && u.altitude < cell.altitude && u.river != &current)
						{
							++dy;
						}
						else if (delta.y() > 0 && d.altitude < cell.altitude && d.river != &current)
						{
							--dy;
						}
					}
				}


				// or flow to the ocean
				if (dx == 0 && dy == 0)
				{
					vector2 delta = { r.moisture - l.moisture, u.moisture - d.moisture };
					bool axis = std::abs(delta.x()) > std::abs(delta.y());
					if (std::abs(delta.x()) == std::abs(delta.y())) axis = even(m_generator) == 0;

					if (axis)
					{
						if (delta.x() > 0 && r.moisture > cell.moisture && r.river != &current)
						{
							++dx;
						}
						else if (delta.x() < 0 && l.moisture > cell.moisture && l.river != &current)
						{
							--dx;
						}
					}
					else
					{
						if (delta.y() > 0 && u.moisture > cell.moisture &&  u.river != &current)
						{
							++dy;
						}
						else if (delta.y() < 0 && d.moisture > cell.moisture && d.river != &current)
						{
							--dy;
						}
					}
				}

				if (dx != 0 || dy != 0)
				{
					generate_river(x + dx, y + dy, size + 1.0, current);
				}
			}
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