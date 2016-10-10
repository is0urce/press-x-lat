// name: map_mapper.hpp
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_CORE_GEN_MAP_MAPPER_HPP
#define PX_CORE_GEN_MAP_MAPPER_HPP

#include <px/core/gen/mapper.hpp>

#include <map>

namespace px
{
	namespace core
	{
		class map_mapper : public mapper
		{
		public:
			typedef map_type::element tile_type;

		public:
			virtual ~map_mapper() {}
			map_mapper() {}

		protected:
			virtual void apply_mapping(fn::build_result &build, map_type &terrain, units_list &units, bool generate_placeables) const override
			{
				build.tiles.enumerate([&](auto const& location, auto const& blueprint_tile) {
					auto &tile = terrain[location];
					auto &img = tile.appearance();

					auto found = m_map.find(blueprint_tile.tile);
					if (found != m_map.end())
					{
						tile = found->second; // pair value
					}
				});
			}

		public:
			void add_rule(fn::build_tile t, map_type::element e)
			{
				m_map[t] = e;
			}
			void remove_rule(fn::build_tile t)
			{
				m_map.erase(t);
			}
			void clear_rules()
			{
				m_map.clear();
			}

		private:
			std::map<fn::build_tile, map_type::element> m_map;
		};
	}
}

#endif