// name: landmark.hpp
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_CORE_LANDMARK_HPP
#define PX_CORE_LANDMARK_HPP

#include <px/rl/entity.hpp>
#include <px/core/image.hpp>

#include <functional>

namespace px
{
	namespace core
	{
		class landmark : public rl::entity
		{
			image appearance;
		};
	}
}

#endif