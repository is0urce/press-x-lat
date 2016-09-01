// name: farm_mapper.hpp
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_CORE_GEN_POI_FARM_MAPPER_HPP
#define PX_CORE_GEN_POI_FARM_MAPPER_HPP

#include <px/core/gen/mapper.hpp>

namespace px
{
	namespace core
	{
		class factory;
		class farm_mapper : public mapper
		{
		public:
			farm_mapper(factory &unit_builder);
			virtual ~farm_mapper();

		protected:
			virtual void apply_mapping(fn::build_result &build, map_type &terrain, units_list &units, bool generate_placeables) const override;

		private:
			factory* m_factory;
		};
	}
}

#endif