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
		template <typename Image>
		class tile : public mass<(unsigned int)traverse::max_value>
		{
		public:
			typedef Image appearance_type;

		public:
			const appearance_type& appearance() const
			{
				return m_image;
			}
			appearance_type& appearance()
			{
				return m_image;
			}

		private:
			Image m_image;
		};
	}
}

#endif