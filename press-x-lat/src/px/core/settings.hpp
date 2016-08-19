// name: settings.hpp
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_CORE_SETTINGS_HPP
#define PX_CORE_SETTINGS_HPP

//#include <px/common/matrix.hpp>
//#include <px/core/image.hpp>
//#include <px/rl/tile.hpp>
//#include <memory>

namespace px
{
	namespace core
	{
		class unit;
		class settings
		{
		public:
			static const unsigned int world_width = 50;
			static const unsigned int world_height = world_width;
			static const unsigned int cell_width = 100;
			static const unsigned int cell_height = cell_width;

			//typedef std::shared_ptr<unit> unit_ptr;
			//typedef rl::tile<image> tile_type;
			//typedef matrix2<tile_type, cell_width, cell_height> map_type;

			//const char* font = "DejaVuSansMono.ttf";
		};
	}
}

#endif