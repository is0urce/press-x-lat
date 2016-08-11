// name: world_cell.hpp
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_FN_RIVER_NET_HPP
#define PX_FN_RIVER_NET_HPP

#include <px/common/matrix.hpp>

#include <px/fn/river.hpp>

namespace px
{
	namespace fn
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
				m_map.enumerate([](int, int, auto& r) {
					r.size = 0;
					r.flow = point2(0, 0);
				});
			}

		private:
			map_type m_map;
		};
	}
}

#endif