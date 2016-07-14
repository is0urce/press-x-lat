// name: scene.hpp
// type: c++ header
// desc: 'scene' class definition
// auth: is0urce

#ifndef PX_CORE_SCENE_H
#define PX_CORE_SCENE_H

#include "location_component.hpp"
#include "unit.hpp"
#include "image.hpp"
#include "world.hpp"

#include <px/rl/tile.hpp>
#include <px/rl/traverse.hpp>
#include <px/common/matrix.hpp>

#include <memory>

namespace px
{
	namespace core
	{
		class scene
		{
		public:
			static const unsigned int cell_width = 10;
			static const unsigned int cell_height = 10;

		public:
			typedef std::shared_ptr<unit> unit_ptr;
			typedef rl::tile<image> tile;
			typedef matrix2<tile, cell_width, cell_height> map;

		private:
			// loading
			point2 m_focus;
			tile m_default;
			std::list<unit_ptr> m_units; // storage container
			map m_terrain;
			world m_world;

		public:
			scene()
				: m_world(0)
			{
				m_world.arrange(point2(0, 0), m_terrain, m_units);
			}
			scene(const scene&) = delete;
			virtual ~scene() {}

		private:
			point2 cell(const point2 &absolute) const;

		public:
			const tile& select(const point2 &position) const
			{
				m_terrain.select(position, m_default);
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
			}

			void clear()
			{
				m_units.clear();
			}
		};
	}
}

#endif