// name: landmark.hpp
// type: c++ header
// desc: struct declaration
// auth: is0urce

#ifndef PX_CORE_LANDMARK_HPP
#define PX_CORE_LANDMARK_HPP

#include <px/common/matrix.hpp>

#include <px/rl/tile.hpp>
#include <px/rl/entity.hpp>
#include <px/core/image.hpp>

#include <px/core/settings.hpp>

#include <functional>
#include <memory>

namespace px
{
	namespace core
	{
		class builder;
		class mapper;

		struct landmark : public rl::entity
		{
		public:
			typedef matrix2<rl::tile<image>, settings::cell_width, settings::cell_height> map_type;

		public:
			void generate(map_type& cell) const
			{
				//mapper->map(builder(cell), cell, units, placeables);
			}

		public:
			image appearance;
			//std::unique_ptr<builder> builder;
			//std::unique_ptr<mapper> mapper;
		};
	}
}

#endif