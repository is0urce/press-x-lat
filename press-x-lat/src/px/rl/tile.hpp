// name: tile.hpp
// type: c++ header
// desc: tile class
// auth: is0urce

#ifndef PX_RL_TILE_HPP
#define PX_RL_TILE_HPP

#include <px/rl/mass.hpp>
#include <px/rl/traverse.hpp>

namespace px
{
	namespace rl
	{
		template <typename Img>
		class tile : public mass<(unsigned int)traverse::max_value>
		{
		private:
			Img m_image;

		public:
			const Img& appearance() const
			{
				return m_image;
			}
			Img& appearance()
			{
				return m_image;
			}
		};
	}
}

#endif