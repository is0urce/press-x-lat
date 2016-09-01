// name: build_result.hpp
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_FN_BUILD_RESULT_HPP
#define PX_FN_BUILD_RESULT_HPP

#include <px/common/point.hpp>
#include <px/common/matrix.hpp>

#include <px/fn/build_tile.hpp>
#include <px/fn/build_placeable.hpp>

#include <list>

namespace px
{
	namespace fn
	{
		struct tile_entry
		{
		public:
			build_tile tile;
			unsigned int group;
			tile_entry(build_tile t, unsigned int g) noexcept
				: tile(t), group(0)
			{}
			tile_entry(build_tile t) noexcept
				: tile_entry(t, 0)
			{}
			tile_entry() = default;
		};

		struct placeable_entry
		{
		public:
			point2 location;
			build_placeable placeable;
			unsigned int group;

			placeable_entry(point2 l, build_placeable p, unsigned int group_index) noexcept
				: location(l), placeable(p), group(group_index)
			{}
			placeable_entry(point2 l, build_placeable p) noexcept
				: placeable_entry(l, p, 0)
			{}
		};

		struct build_result
		{
			matrix2<tile_entry> tiles;
			std::list<placeable_entry> placeables;

			bool exists(point2 const& location) const noexcept
			{
				for (auto const& placeable : placeables)
				{
					if (placeable.location == location) return true;
				}
				return false;
			}
		};
	}
}

#endif