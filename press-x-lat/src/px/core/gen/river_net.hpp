// name: river_net.hpp
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_CORE_RIVER_NET_HPP
#define PX_CORE_RIVER_NET_HPP

#include <px/common/matrix.hpp>

#include <px/core/gen/river.hpp>

namespace px
{
	namespace core
	{
		class river_net
		{
		public:
			typedef matrix2<river> map_type;

		public:
			void resize(int w, int h)
			{
				m_map.resize(w, h);
			}
			void clear()
			{
				//m_map.enumerate([](auto, auto, auto& r) {
				//	//r.size = 0;
				//	//r.flow = point2(0, 0);
				//});
			}

		private:
			map_type m_map;
		};
	}
}

#endif