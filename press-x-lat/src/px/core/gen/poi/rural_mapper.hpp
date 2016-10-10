// name: rural_mapper.hpp
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_CORE_GEN_POI_RURAL_MAPPER_HPP
#define PX_CORE_GEN_POI_RURAL_MAPPER_HPP

#include <px/core/gen/map_mapper.hpp>

namespace px
{
	namespace core
	{
		class factory;
		class rural_mapper : public map_mapper
		{
		public:
			virtual ~rural_mapper();
			rural_mapper(factory &unit_builder);

		protected:
			virtual void apply_mapping(fn::build_result &build, map_type &terrain, units_list &units, bool generate_placeables) const override;

		private:
			factory* m_factory;
		};
	}
}

#endif