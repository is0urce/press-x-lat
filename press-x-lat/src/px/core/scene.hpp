// name: scene.hpp
// type: c++ header
// desc: 'scene' class definition
// auth: is0urce

#ifndef PX_CORE_SCENE_H
#define PX_CORE_SCENE_H

#include "location_component.hpp"
#include "unit.hpp"
#include "image.hpp"

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
			qtree<location_component*>* m_space;

			// loading
			point2 m_focus;
			tile m_default;
			std::list<unit_ptr> m_units; // storage container

		public:
			scene(qtree<location_component*> &space) : m_space(&space) {}
			virtual ~scene() {}

		private:
			point2 cell(const point2 &absolute) const;

		public:
			void focus(point2 position);
			void focus(point2 position, bool force);
			const tile& select(const point2 &position) const;
			bool transparent(const point2 &point) const;
			bool traversable(const point2 &point, rl::traverse layer) const;

			void add(unit_ptr unit)
			{
				m_units.push_back(unit);
			}

			void clear();
		};
	}
}

#endif