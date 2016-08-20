// name: farm_mapper.hpp
// type: c++ header
// desc: struct declaration
// auth: is0urce

#ifndef PX_CORE_GEN_POI_FARM_MAPPER_HPP
#define PX_CORE_GEN_POI_FARM_MAPPER_HPP


#include <px/fn/bsp.hpp>
#include <px/core/gen/build_result.hpp>

#include <px/core/gen/mapper.hpp>

namespace px
{
	namespace core
	{
		class farm_mapper : public mapper
		{
		public:
			farm_mapper() {}
			virtual ~farm_mapper() {}

		protected:
			void apply_mapping(build_result &build, map_type &terrain, units_list &units, bool generate_placeables) const override
			{
				build.tiles.enumerate([&](auto const& location, auto const& blueprint_tile) {
					auto &tile = terrain[location];
					auto &img = tile.appearance();

					switch (blueprint_tile)
					{
					case build_tile::wall:
					case build_tile::wall_inside:
					case build_tile::wall_outside:
						tile.make_blocking();
						tile.make_opaque();
						img.glyph = 0x2317; // '#'
						img.tint = { 1, 1, 1 };
						img.bg = { 0, 0, 0 };
						break;
					case build_tile::floor:
						tile.make_traversable();
						tile.make_transparent();
						img.glyph = '.';
						img.tint = { 1, 1, 1 };
						img.bg = { 0, 0, 0 };
						break;
					case build_tile::soil:
						tile.make_traversable();
						tile.make_transparent();
						img.glyph = '"';
						img.tint = { 0, 1, 0 };
						img.bg = { 0.25, 0.25, 0 };
						break;
					case build_tile::no_change:
						break;
					default:
						break;
					}
				});
			}
		};

	}
}

#endif