// name: landmark.cpp
// type: c++ header
// desc: class methods
// auth: is0urce

#include "landmark.hpp"

#include <px/common/rectangle.hpp>

#include <px/fn/build_result.hpp>

#include <px/core/gen/builder.hpp>
#include <px/core/gen/mapper.hpp>

namespace px
{
	namespace core
	{
		landmark::landmark() {}
		landmark::~landmark() {}

		void landmark::generate(unsigned int seed, map_type &map, std::list<unit_record> &units, bool generate_placeables) const
		{
			if (m_builder && m_mapper)
			{
				fn::build_result result;
				result.tiles.resize(map.range());

				m_builder->run(seed, map.range(), result);
				m_mapper->map(result, map, units, generate_placeables);
			}
		}

		void landmark::set_pipeline(std::unique_ptr<builder> && b, std::unique_ptr<mapper> && m)
		{
			std::swap(m_builder, b);
			std::swap(m_mapper, m);
		}
	}
}