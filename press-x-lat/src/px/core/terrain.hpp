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
		class terrain
		{
		public:
			static const unsigned int cell_width = world::cell_width;
			static const unsigned int cell_height = world::cell_height;
			static const unsigned int sight_reach = 1;
			static const unsigned int sight_width = 1 + sight_reach * 2;

		public:
			typedef std::shared_ptr<unit> unit_ptr;
			typedef rl::map_stream<image, unit_ptr, cell_width, cell_height> stream_map;
			typedef stream_map::map map;
			typedef stream_map::tile tile;
			typedef stream_map::unit_list units;
			typedef std::unique_ptr<map> stream_ptr;

		private:
			// loading
			point2 m_focus;
			tile m_default;
			std::list<unit_ptr> m_units; // storage container
			stream_map m_terrain;
			matrix2<stream_ptr, sight_width, sight_width> m_maps;
			world m_world;

		public:
			terrain()
				: m_world(0)
			{
				m_terrain.load_stream([&](map &m, units &u) { m_world.arrange(point2(0, 0), m, u); });

				m_default.appearance() = { '.', { 0, 0, 0, 0 } };
				m_default.make_wall();
			}
			virtual ~terrain()
			{
			}
			terrain(const terrain&) = delete;

		private:
			point2 cell(const point2 &absolute) const;

		public:
			void focus(point2 position)
			{
				m_focus = position;
			}
			const tile& select(const point2 &position) const
			{
				if (m_terrain.loaded())
				{
					return m_terrain->select(position, m_default);
				}
				else
				{
					return m_default;
				}
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