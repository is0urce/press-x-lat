// name: rendering_system.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_CORE_RENDERING_SYSTEM_HPP
#define PX_CORE_RENDERING_SYSTEM_HPP

#include <px/es/i_system.hpp>
#include <px/es/manager.hpp>
#include <px/shell/canvas.hpp>

#include "image_component.hpp"
#include "location_component.hpp"

namespace px
{
	namespace core
	{
		class rendering_system
			: public es::i_system
			, public es::manager<image_component, 10000>
		{
		private:
			shell::canvas* m_canvas;

		public:
			rendering_system(shell::canvas& cnv)
				: m_canvas(&cnv)
			{
			}
			virtual ~rendering_system()
			{
			}

		protected:
			virtual void update_system() override
			{
				enumerate([this](const image_component &img)
				{
					if (img.active())
					{
						location_component* location = (location_component*)img;
						if (location)
						{
							auto current = location->current();

							m_canvas->write({ current.x(), -current.y() }, img.glyph, img.tint);
						}
					}
				});
			}

		public:
			auto make_sprite(unsigned int glyph)-> decltype(make())
			{
				auto sprite = make();
				sprite->glyph = glyph;
				return sprite;
			}
		};
	}
}

#endif