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
		class house_builder : public mapper
		{
		public:
			house_builder() {}
			~house_builder() {}

		protected:
			void apply_mapping(build_result &build, map_type &terrain, units_list &units, bool generate_placeables) const override
			{

				build.tiles.enumerate([&](auto x, auto y, auto const& blueprint_tile) {
					auto &tile = terrain[point2(x, y)];
					auto &img = tile.appearance();

					switch (blueprint_tile)
					{
					case build_tile::wall:
						tile.make_blocking();
						tile.make_opaque();
						img.glyph = '#';
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
					case build_tile::no_change:
					default:
						break;
					}
				});
			}
		};

	}
}

#endif