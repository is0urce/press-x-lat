// name: terrain.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_CORE_TERRAIN_HPP
#define PX_CORE_TERRAIN_HPP

#include <px/core/image.hpp>
#include <px/core/world.hpp>

#include <px/rl/tile.hpp>
#include <px/rl/traverse.hpp>
#include <px/rl/map_stream.hpp>
#include <px/common/matrix.hpp>

#include <memory>

namespace px
{
	namespace core
	{
		class unit;
		class world;

		class terrain
		{
		public:
			static const unsigned int cell_width = world::cell_width;
			static const unsigned int cell_height = world::cell_height;
			static const unsigned int sight_reach = 1;
			static const unsigned int sight_width = 1 + sight_reach * 2;
			typedef std::shared_ptr<unit> unit_ptr;
			typedef rl::map_stream<image, unit_ptr, cell_width, cell_height> stream_type;
			typedef stream_type::map_type map_type;
			typedef stream_type::tile_type tile_type;
			typedef stream_type::unit_list units;
			typedef std::unique_ptr<stream_type> stream_ptr;
			typedef matrix2<stream_ptr, sight_width, sight_width> maps_type;

		public:
			void focus(point2 position);
			const tile_type& select(const point2 &position) const;
			bool transparent(const point2 &point) const;
			bool traversable(const point2 &point, rl::traverse layer) const;
			void add(unit_ptr unit);
			void handle_units();

			template <typename Op>
			void enumerate(Op&& fn)
			{
				std::for_each(std::begin(m_units), std::end(m_units), std::forward<Op>(fn));
			}

		public:
			terrain(world &w);
			virtual ~terrain();
			terrain(const terrain&) = delete;

		private:
			void load_stream(point2 cell, stream_ptr& area);
			void merge(stream_type& map);

		private:
			// loading
			tile_type m_default;
			maps_type m_maps;
			point2 m_focus; // absolute world cell coordinate
			units m_units; // storage container
			world* m_world;
		};
	}
}

#endif