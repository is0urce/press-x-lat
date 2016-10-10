// name: world.cpp
// type: c++ header
// desc: class methods implementation
// auth: is0urce

#include "world.hpp"

#include <px/common/vector.hpp>

#include <px/fn/perlin.hpp>

#include <px/core/gen/world_cell.hpp>
#include <px/core/gen/landmark.hpp>

#include <px/core/gen/poi/farm_builder.hpp>
#include <px/core/gen/poi/graveyard_builder.hpp>
#include <px/core/gen/poi/rural_mapper.hpp>

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

			// lerp between w0 and w1, w0 < w < w1
			template <typename T, typename M>
			static T lerp(T a, T b, M w, M w0, M w1)
			{
				M ww = (w - w0) / (w1 - w0);
				return ww < 0 ? a : ww > 1 ? b : lerp(a, b, (w - w0) / (w1 - w0));
			}
		}

		world::world(factory &unit_builder) : m_factory(&unit_builder)
		{
			clear_cell(m_outer);
		}
		world::~world() {}

		void world::clear()
		{
			m_map.enumerate([this, size = m_map.size()](auto location, auto& cell) {
				clear_cell(cell);
				cell.location = location;
				cell.seed = static_cast<unsigned int>(m_seed + location.y() * size + location.x());
			});
			m_rivers.clear();
			m_cities.clear();
			m_spawn = { 0, 0 };
		}

		void  world::clear_cell(cell_type &cell)
		{
			// out-of-border cell proprieties
			m_outer.seed = 0;
			m_outer.altitude = -1;
			m_outer.gradient = { 0, 0 };
			m_outer.moisture = 0;
			m_outer.temperature = 0;
			m_outer.river = nullptr;
			m_outer.river_size = 0;
			m_outer.landmark = nullptr;
			m_outer.level = 0;
			m_outer.generated = true;
		}

		void world::resize(size_t width, size_t height)
		{
			m_map.resize(width, height);
		}

		void world::generate(unsigned int seed)
		{
			// setup and warmup random generator
			m_seed = seed;
			std::array<unsigned int, 624> seed_data;
			std::iota(std::begin(seed_data), std::end(seed_data), m_seed);
			std::seed_seq state(std::begin(seed_data), std::end(seed_data));
			m_generator.seed(state);
			m_generator.discard(rng_type::state_size);

			clear();
			generate_landmass();
			generate_climate(3);
			generate_biomes();
			generate_civilisation(2);
			generate_appearance();
		}

		// generate altitudes [-1+..1], sea level is 0
		void world::generate_landmass()
		{
			fn::perlin<perlin_width, perlin_height> noise(m_generator);

			double w = static_cast<double>(m_map.width());
			double h = static_cast<double>(m_map.height());
			double mx = (perlin_width - 1) / w;
			double my = (perlin_height - 1) / h;

			double max_peak = -1.0;

			// calculate altitudes
			vector2 center(w / 2.0f, h / 2.0f);
			double size = (std::min)(w, h);
			m_map.enumerate([&](auto const& location, auto &cell)
			{
				double noise_magnitude = noise.sample(mx * location.x(), my * location.y(), perlin_samples);
				double distance_to_center = center.distance(vector2(location));
				double cone_magnitude = (size - distance_to_center * 4) / size; // [-1..1]

				cell.altitude = std::atan(cone_magnitude) + noise_magnitude;
				cell.generated = false;

				max_peak = (std::max)(max_peak, cell.altitude);
			});

			// normalise, calculate gradient
			m_map.enumerate([mx, my, max_peak, this](auto const& location, auto& cell)
			{
				cell.altitude = cell.altitude / max_peak;

				auto dx = m_map.select({ location.x() + 1, location.y() }, m_outer).altitude - m_map.select({ location.x() - 1, location.y() }, m_outer).altitude;
				auto dy = m_map.select({ location.x(), location.y() + 1 }, m_outer).altitude - m_map.select({ location.x(), location.y() - 1 }, m_outer).altitude;

				cell.gradient = { dx, dy };
			});
		}

		void world::generate_climate(double rivers)
		{
			// fill moisture from ocean
			m_rivers.clear();
			m_map.enumerate([h = m_map.height()](auto const& location, auto& cell)
			{
				cell.moisture = cell.altitude <= 0 ? 1.0 : 0.0;
				cell.temperature = -0.25 + 1.0 * (h - location.y()) / h - cell.altitude * 0.5;
				cell.river = nullptr;
				cell.river_size = 0;
			});
			expand_moisture();

			// generate rivers
			std::uniform_int_distribution<size_t> rand_x(0, m_map.width() - 1);
			std::uniform_int_distribution<size_t> rand_y(0, m_map.height() - 1);
			unsigned int n = 0;
			unsigned int tries = 0;
			while (n < rivers && tries < max_try_stop)
			{
				++tries;
				int x = static_cast<int>(rand_x(m_generator));
				int y = static_cast<int>(rand_y(m_generator));

				auto &cell = m_map[point2(x, y)];
				if (cell.river_size <= 0 && cell.altitude > 0.35)
				{
					river r;
					r.set_tag(std::string("river") + std::to_string(n));

					m_rivers.emplace_back(r);
					create_river(x, y, cell.moisture, m_rivers.back());
					++n;
				}
			}

			// fill moisture from rivers
			m_map.enumerate([](auto const&, auto& cell)
			{
				cell.moisture = cell.river_size > 0 ? 1.0 : cell.altitude < 0 ? 0.0125 : 0.0;
			});
			expand_moisture();
		}

		void world::generate_biomes()
		{
			// ocean
			m_ocean.clear();
			m_ocean.appearance = { '~',{ 1.0, 1.0, 1.0 },{ 0.25, 0.25, 1.0 } };
			m_ocean.ground.make_blocking();
			m_ocean.ground.make_transparent();
			m_ocean.ground.appearance() = m_ocean.appearance;

			// mountain biomes
			m_mountain_rock.clear();
			m_mountain_rock.ground.make_blocking();
			m_mountain_rock.ground.make_opaque();
			m_mountain_rock.ground.appearance() = { '^', color::black(), { 0.5, 0.5, 0.5 } };

			m_mountain_ice = m_mountain_rock;
			m_mountain_ice.ground.appearance() = { '^', color::black(), color(0.9, 0.9, 0.9) };

			// snow biomes
			m_tundra.clear();
			m_tundra.ground.appearance() = { '.', { 0.0, 0.0, 0.0 }, { 0.9, 0.9, 0.9 } };

			m_taiga.clear();
			m_taiga.ground.appearance() = { 9650, { 0.0, 0.25, 0.0 }, { 0.9, 0.9, 0.9 } };

			// plain biomes
			m_plains.clear();
			m_plains.ground.appearance() = { '"', { 0.0, 0.25, 0.0 }, { 0.0, 0.5, 0.0 } };

			m_forest.clear();
			m_forest.ground.appearance() = { 9650, color(0.0, 0.0, 0.0), { 0.0, 0.5, 0.0 } };

			// dry biomes
			m_desert.clear();
			m_desert.ground.appearance() = { '.', color(0.0, 0.0, 0.0), color(0.8, 0.8, 0.5) };

			m_map.enumerate([this](auto const& location, auto& cell)
			{
				assign_biome(cell);
			});
		}

		void world::generate_civilisation(unsigned int cities)
		{
			m_cities.clear();
			m_map.enumerate([](auto const&, auto& cell)
			{
				cell.landmark = nullptr;
			});

			// generate cities
			std::uniform_int_distribution<size_t> rand_x(0, m_map.width() - 1);
			std::uniform_int_distribution<size_t> rand_y(0, m_map.height() - 1);
			unsigned int n = 0;
			unsigned int tries = 0;
			while (n < cities && tries < max_try_stop)
			{
				++tries;
				unsigned int x = static_cast<unsigned int>(rand_x(m_generator));
				unsigned int y = static_cast<unsigned int>(rand_y(m_generator));

				point2 location = point2(x, y);
				auto &cell = m_map[location];
				int distance = static_cast<int>(m_map.width()) * 2 + static_cast<int>(m_map.height()) * 2 + 5; // something big
				std::for_each(m_cities.begin(), m_cities.end(), [&distance, &location](auto& c) {
					distance = (std::min)(distance, location.king_distance(c.center_location));
				});

				if (cell.landmark == 0 && cell.altitude > 0 && cell.altitude < 0.5 && cell.moisture > 0 && distance > 5)
				{
					++n;
					city capital;
					capital.center_location = location;
					capital.set_tag(std::string("city") + std::to_string(n));
					m_cities.push_back(capital);

					cell.landmark = std::make_unique<landmark>();
					cell.landmark->set_name(std::string("landmark") + std::to_string(n));
					m_spawn = location;
				}
			}

			// generate other landmarks
			int count = 0;
			m_map.enumerate([&count, this](auto const& location, auto & cell)
			{
				if (!cell.landmark && cell.altitude > 0)
				{
					cell.landmark = std::make_unique<landmark>();
					cell.landmark->set_name(std::string("point-of-interest#") + std::to_string(++count));

					int seed = cell.seed;
					rng_type random(seed);
					if (std::uniform_int_distribution<int>{0, 1}(random) == 0)
					{
						cell.landmark->set_pipeline(std::make_unique<farm_builder>(), std::make_unique<rural_mapper>(*m_factory));
					}
					else cell.landmark->set_pipeline(std::make_unique<graveyard_builder>(), std::make_unique<rural_mapper>(*m_factory));
				}
			});
		}

		void world::generate_appearance()
		{
			m_map.enumerate([w = m_map.width(), h = m_map.height(), this](auto const&, auto& cell)
			{
				cell.img = cell.biome.ground.appearance();

				// some tweaks to map colors
				if (cell.altitude <= 0)
				{
					cell.img.bg = color(0.25 + cell.altitude / 4.0, 0.25 + cell.altitude / 4.0, 1 + cell.altitude / 4.0); // -1,0 - > [0, 0.5]
				}
				else
				{
					if (cell.altitude < 0.5)
					{
						if (cell.temperature < 0)
						{
							//cell.img.bg = lerp(color(0.9, 0.9, 0.9), color(1, 1, 1), cell.altitude, 0.0, 1.0);
						}
						else if (cell.temperature > 0.25 && cell.moisture < 0.0625)
						{
							//cell.img.bg = lerp(color(0.8, 0.8, 0.0), color(0.8, 0.8, 0.5), 1 - cell.altitude, 0.5, 1.0);
						}
						else
						{
							//cell.img.bg = lerp(color(0.0, 0.5, 0.0), color(0, 0.75, 0), cell.altitude, 0.0, 1.0);
						}
					}
					else // mountains
					{
						if (cell.temperature <= 0)
						{
							//cell.img.tint = lerp(color(0, 0, 0), color(0.75, 0.75, 1), cell.altitude, 0.5, 1.0);
							//cell.img.bg = lerp(color(0.9, 0.9, 0.9), color(1, 1, 1), cell.altitude, 0.75, 1.0);
						}
					}

					if (cell.river_size > 0)
					{
						cell.img.glyph = '~';
						cell.img.tint = { 0, 0, 1 };
					}
					if (cell.landmark)
					{
						//cell.img.glyph = '*';
						//cell.img.tint = { 1, 0, 1 };
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
				m_map.enumerate([&change, this](auto const& location, auto& cell)
				{
					auto &u = m_map.select(location.moved_axis<1>(1), m_outer);
					auto &d = m_map.select(location.moved_axis<1>(-1), m_outer);
					auto &l = m_map.select(location.moved_axis<0>(-1), m_outer);
					auto &r = m_map.select(location.moved_axis<0>(1), m_outer);

					double moisture = (std::max)((std::max)(u.moisture, d.moisture), (std::max)(l.moisture, r.moisture)) / 2;

					if (cell.moisture < moisture)
					{
						cell.moisture = moisture;
						change = true;
					}
				});
			}
		}

		void world::create_river(int x, int y, double size, river& current)
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
					create_river(x + dx, y + dy, size + 1.0, current);
				}
			}
		}

		// setup biome of cell, depending on other factors as moisture and temperature
		void world::assign_biome(cell_type &cell)
		{
			if (cell.altitude <= 0)
			{
				cell.biome = m_ocean;
			}
			else if (cell.altitude < 0.5)
			{
				if (cell.temperature < 0)
				{
					cell.biome = cell.moisture < 0.0625 ? m_tundra : m_taiga;
				}
				else if (cell.temperature > 0.25 && cell.moisture < 0.0625)
				{
					cell.biome = m_desert;
				}
				else
				{
					cell.biome = cell.moisture < 0.0625 ? m_plains : m_forest;
				}
			}
			else // mountains
			{
				cell.biome = cell.temperature > 0 ? m_mountain_rock : m_mountain_ice;
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
		point2 world::spawn() const noexcept
		{
			return m_spawn;
		}
		world::cell_type& world::select(point2 const& location)
		{
			return m_map.select(location, m_outer);
		}
		world::cell_type const& world::select(point2 const& location) const
		{
			return m_map.select(location, m_outer);
		}
	}
}