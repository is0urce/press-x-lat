// name: settings.hpp
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_CORE_SETTINGS_HPP
#define PX_CORE_SETTINGS_HPP

namespace px
{
	namespace core
	{
		class settings
		{
		public:
			static const unsigned int world_width = 50;
			static const unsigned int world_height = world_width;
			static const unsigned int cell_width = 100;
			static const unsigned int cell_height = cell_width;

			//const char* font = "DejaVuSansMono.ttf";
		};
	}
}

#endif