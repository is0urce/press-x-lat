// name: mapper.hpp
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_CORE_GEN_MAPPER_HPP
#define PX_CORE_GEN_MAPPER_HPP

#include <px/common/matrix.hpp>

#include <px/rl/tile.hpp>

#include <px/core/image.hpp>
#include <px/core/settings.hpp>

#include <list>

namespace px
{
	namespace core
	{
		struct unit_record;
		struct build_result;

		class mapper
		{
		public:
			typedef matrix2<rl::tile<image>, settings::cell_width, settings::cell_height> map_type;
			typedef std::list<unit_record> units_list;

		public:
			void map(build_result &build, map_type &terrain, units_list &units, bool generate_placeables) const
			{
				apply_mapping(build, terrain, units, generate_placeables);
			}

		public:
			mapper() {}
			virtual ~mapper() = 0 {}

		protected:
			virtual void apply_mapping(build_result &build, map_type &terrain, units_list &units, bool generate_placeables) const = 0;
		};
	}
}

#endif