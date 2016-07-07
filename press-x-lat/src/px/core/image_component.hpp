// name: image_component.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_CORE_IMAGE_COMPONENT_HPP
#define PX_CORE_IMAGE_COMPONENT_HPP

#include <px/es/i_component.hpp>
#include "image.hpp"

namespace px
{
	namespace core
	{
		class image_component : public es::i_component, public image
		{
		public:
			virtual ~image_component() {}
		};
	}
}

#endif