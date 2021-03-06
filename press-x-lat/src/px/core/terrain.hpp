// name: terrain.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_CORE_TERRAIN_HPP
#define PX_CORE_TERRAIN_HPP

#include <px/common/matrix.hpp>

#include <px/rl/tile.hpp>
#include <px/rl/traverse.hpp>
#include <px/rl/map_stream.hpp>

#include <px/core/image.hpp>
#include <px/core/unit_record.hpp>
#include <px/core/settings.hpp>

#include <memory>

namespace px
{
	namespace core
	{
		class unit;
		class terrain_director;

		class terrain
		{
		public:
			static const unsigned int sight_reach = 1;
			static const unsigned int sight_width = 1 + sight_reach * 2;
			typedef std::shared_ptr<unit> unit_ptr;
			typedef rl::map_stream<image, unit_record, settings::cell_width, settings::cell_height> stream_type;
			typedef stream_type::tile_type tile_type;
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
			terrain(terrain_director &cell_manager);
			virtual ~terrain();
			terrain(const terrain&) = delete;

		private:
			void load_stream(point2 cell, stream_ptr& area);
			void splice(stream_type& map, const point2& cell);

		private:
			// loading
			tile_type m_default;
			maps_type m_maps;
			point2 m_focus; // absolute world cell coordinate
			std::list<unit_ptr> m_units; // storage container
			terrain_director* m_world;
		};
	}
}

#endif