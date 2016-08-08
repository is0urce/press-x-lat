// name: terrain.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_CORE_TERRAIN_HPP
#define PX_CORE_TERRAIN_HPP

#include <px/common/vector.hpp>

#include <px/core/sys/location_component.hpp>
#include "unit.hpp"
#include "image.hpp"
#include "world.hpp"

#include <px/rl/tile.hpp>
#include <px/rl/traverse.hpp>
#include <px/rl/map_stream.hpp>
#include <px/common/matrix.hpp>

#include <memory>
#include <mutex>

namespace px
{
	namespace core
	{
		namespace
		{
			static const unsigned int cell_width = world::cell_width;
			static const unsigned int cell_height = world::cell_height;
			static const unsigned int sight_reach = 1;
			static const unsigned int sight_width = 1 + sight_reach * 2;
			point2 cell_range = point2(cell_width, cell_height);
			point2 sight_center = point2(sight_reach, sight_reach);
		}

		class terrain
		{
		public:
			typedef std::shared_ptr<unit> unit_ptr;
			typedef rl::map_stream<image, unit_ptr, cell_width, cell_height> stream;
			typedef stream::map map;
			typedef stream::tile tile;
			typedef stream::unit_list units;
			typedef std::unique_ptr<stream> stream_ptr;
			typedef matrix2<stream_ptr, sight_width, sight_width> maps;

		private:
			// loading
			tile m_default;
			maps m_maps;
			point2 m_focus; // absolute world cell coordinate
			units m_units; // storage container
			world* m_world;

		public:
			terrain(world &w) : m_world(&w)
			{
				m_default.appearance() = { '.', { 0, 0, 0, 0 } };
				m_default.make_wall();
			}
			virtual ~terrain()
			{
			}
			terrain(const terrain&) = delete;

		private:
			static point2 divide(point2 a, point2& remainder)
			{
				point2 div = (vector2(a) / cell_range).floor();
				remainder = a - div * cell_range;
				return div;
			}
			void load_stream(point2 world_cell, stream_ptr& area)
			{
				if (area) throw std::runtime_error("px::terrain::load - not null");
				area = std::make_unique<stream>();
				area->load_stream([world_cell, this](stream::map &m, units &u) { m_world->arrange(world_cell, m, u); });
			}
			void merge(stream& map)
			{
				units list;
				map.merge(list);
				for (auto u : list)
				{
					add(u);
				}
			}

		public:
			void focus(point2 position)
			{
				// calculate new coordinates
				point2 relative;
				point2 focus = divide(position, relative);

				// shift maps
				if (m_focus != focus)
				{
					maps origin;
					origin.swap(m_maps);

					// copy old
					point2 shift = focus - m_focus;
					m_maps.enumerate([&](unsigned int cell_x, unsigned int cell_y, stream_ptr &map)
					{
						auto index = shift + point2(cell_x, cell_y);
						if (origin.contains(index))
						{
							std::swap(map, origin[index]);
						}
					});

					// should clear not swapped destroying maps as they can have not treminated threads
					origin.enumerate([&](unsigned int x, unsigned int y, stream_ptr &map)
					{
						if (map && map->pending())
						{
							merge(*map);
						}
					});

					m_focus = focus;
				}

				// post update maps
				m_maps.enumerate([&](unsigned int x, unsigned int y, stream_ptr &map)
				{
					point2 world_cell = m_focus - sight_center + point2(x, y);

					// create maps if required
					if (!map)
					{
						load_stream(world_cell, map);
					}

					// need central cell loaded
					if (world_cell == m_focus)
					{
						map->wait();
					}

					// join loaded maps
					if (map->pending() && map->loaded())
					{
						merge(*map);
					}
				});
			}
			const tile& select(const point2 &position) const
			{
				point2 relative;
				point2 cell = divide(position, relative) - m_focus + sight_center;

				if (m_maps.contains(cell))
				{
					const stream_ptr &map = m_maps[cell];
					if (map && !map->pending())
					{
						return (*map)->at(relative);
					}
				}
				return m_default;
			}
			bool transparent(const point2 &point) const
			{
				return select(point).transparent();
			}
			bool traversable(const point2 &point, rl::traverse layer) const
			{
				return select(point).traversable(layer);
			}

			void add(unit_ptr unit)
			{
				unit->activate();
				m_units.push_back(unit);
			}

			template <typename Op>
			void enumerate(Op&& fn)
			{
				for (auto it = m_units.begin(), last = m_units.end(); it != last; ++it)
				{
					std::forward<Op>(fn)(*it);
				}
			}

			void handle_units()
			{
				for (auto it = m_units.begin(), last = m_units.end(); it != last; )
				{
					switch ((*it)->get_persistency())
					{
					case persistency::permanent:
						++it;
						break;
					case persistency::destroying:
						if ((*it)->decay_timer() == 0)
						{
							it = m_units.erase(it);
						}
						else
						{
							(*it)->decay();
							++it;
						}
						break;
					case persistency::temporary:
						++it;
						break;
					case persistency::serialized:
						++it;
						break;
					default:
						break;
					}
				}
			}
		};
	}
}

#endif