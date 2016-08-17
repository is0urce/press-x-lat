// name: terrain.cpp
// type: c++ header
// desc: class methods implementation
// auth: is0urce

#include "terrain.hpp"

#include <px/common/vector.hpp>
#include <px/core/unit.hpp>
#include <px/core/terrain_director.hpp>

#include <px/core/sys/location_component.hpp>

namespace px
{
	namespace core
	{
		namespace
		{
			static const point2 cell_range = point2(settings::cell_width, settings::cell_height);
			static const point2 sight_center = point2(terrain::sight_reach, terrain::sight_reach);

			point2 divide(point2 a, point2 &remainder)
			{
				point2 div = (vector2(a) / cell_range).floor();
				remainder = a - div * cell_range;
				return div;
			}
		}
		terrain::terrain(terrain_director &w) : m_world(&w)
		{
			m_default.appearance() = { '.',{ 0, 0, 0, 0 } };
			m_default.make_wall();
		}
		terrain::~terrain()
		{
		}

		void terrain::load_stream(point2 cell, stream_ptr& area)
		{
			if (area) throw std::runtime_error("px::terrain::load - not null");
			area = std::make_unique<stream_type>();
			area->load_stream([cell, this](auto &m, auto &u) { m_world->generate_cell(cell, m, u); });
		}
		void terrain::splice(stream_type& map, const point2 &cell)
		{
			stream_type::unit_list list;
			map.splice_into(list);

			std::for_each(list.begin(), list.end(), [&](unit_record &record) {
				record.location->move(cell_range * cell + record.location->current());
				add(record.unit);
			});
		}

		void terrain::focus(point2 position)
		{
			// calculate new coordinates
			point2 relative;
			point2 focus = divide(position, relative);

			// shift maps
			if (m_focus != focus)
			{
				maps_type origin;
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
					point2 world_cell = m_focus - sight_center + point2(x, y);
					if (map && map->pending())
					{
						splice(*map, world_cell);
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
					splice(*map, world_cell);
				}
			});
		}
		const terrain::tile_type& terrain::select(const point2 &position) const
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
		bool terrain::transparent(const point2 &point) const
		{
			return select(point).transparent();
		}
		bool terrain::traversable(const point2 &point, rl::traverse layer) const
		{
			return select(point).traversable(layer);
		}

		void terrain::add(unit_ptr unit)
		{
			unit->activate();
			m_units.push_back(unit);
		}

		void terrain::handle_units()
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
	}
}