// name: landmark.cpp
// type: c++ header
// desc: class methods
// auth: is0urce

#include "landmark.hpp"

#include <px/common/rectangle.hpp>

#include <px/core/gen/builder.hpp>
#include <px/core/gen/mapper.hpp>

#include <px/core/gen/build_result.hpp>

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
				build_result result;
				result.tiles.resize(map.range());

				m_builder->build(seed, map.range(), result);
				m_mapper->map(result, map, units, generate_placeables);
			}
		}
	}
}