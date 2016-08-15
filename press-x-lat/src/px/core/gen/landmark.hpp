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

namespace px
{
	namespace core
	{
		struct landmark : public rl::entity
		{
		public:
			typedef rl::tile<image> tile_type;
			typedef matrix2<tile_type, settings::cell_width, settings::cell_height> map_type;
			typedef std::function<void(map_type&)> generator_fn;

		public:
			void generate(map_type& cell) const
			{
				if (generator)
				{
					generator(cell);
				}
			}

		public:
			image appearance;
			generator_fn generator;
		};
	}
}

#endif