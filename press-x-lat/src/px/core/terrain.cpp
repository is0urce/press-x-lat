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
			m_default.appearance() = { '.', { 0, 0, 0, 0 } };
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
				m_maps.enumerate([&](auto location, stream_ptr &map)
				{
					location += shift;

					if (origin.contains(location))
					{
						std::swap(map, origin[location]);
					}
				});

				// should clear not swapped destroying maps as they can have not treminated threads
				origin.enumerate([&](auto const& location, stream_ptr &map)
				{
					if (map && map->pending())
					{
						splice(*map, location + m_focus - sight_center);
					}
				});

				m_focus = focus;
			}

			// post update maps
			m_maps.enumerate([&](auto location, stream_ptr &map)
			{
				location += m_focus - sight_center;

				// create maps if required
				if (!map)
				{
					load_stream(location, map);
				}

				// need central cell loaded
				if (location == m_focus)
				{
					map->wait();
				}

				// join loaded maps
				if (map->pending() && map->loaded())
				{
					splice(*map, location);
				}
			});
		}
		const terrain::tile_type& terrain::select(point2 const& position) const
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
		bool terrain::transparent(point2 const& point) const
		{
			return select(point).transparent();
		}
		bool terrain::traversable(point2 const& point, rl::traverse layer) const
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