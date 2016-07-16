// name: terrain.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_CORE_TERRAIN_HPP
#define PX_CORE_TERRAIN_HPP

#include "location_component.hpp"
#include "unit.hpp"
#include "image.hpp"
#include "world.hpp"

#include <px/rl/tile.hpp>
#include <px/rl/traverse.hpp>
#include <px/rl/map_stream.hpp>
#include <px/common/matrix.hpp>

#include <memory>

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

		private:
			// loading
			point2 m_focus;
			tile m_default;
			units m_units; // storage container
			matrix2<stream_ptr, sight_width, sight_width> m_maps;
			world m_world;

		public:
			terrain()
				: m_world(0)
			{
				m_maps.enumerate([&](unsigned int x, unsigned int y, stream_ptr &map) 
				{
					map = std::make_unique<stream>();
					start_loading(point2(x, y) - sight_center, *map);
				});

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
			void start_loading(point2 cell, stream& area)
			{
				area.load_stream([&](stream::map &m, units &u) { m_world.arrange(cell, m, u); });
			}

		public:
			void focus(point2 position)
			{
				point2 remainder;
				point2 div = divide(position, remainder);
				if (m_focus != div)
				{
					m_focus = div;
				}
			}
			const tile& select(const point2 &position) const
			{
				point2 relative;
				point2 div = divide(position, relative);
				div.move(sight_center);

				if (m_maps.contains(div))
				{
					const stream &map = *m_maps[div];
					if (map.loaded() && !map.pending())
					{
						return map->at(relative);
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
				m_units.push_back(unit);
				unit->activate();
			}
		};
	}
}

#endif