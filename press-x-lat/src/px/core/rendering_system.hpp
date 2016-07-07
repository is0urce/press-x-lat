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

namespace px
{
	namespace core
	{
		class rendering_system : public es::i_system, protected es::manager<image_component, 10000>
		{
		private:
			shell::canvas* m_canvas;

		public:
			rendering_system(shell::canvas* c) : m_canvas(c) {}
			virtual ~rendering_system() {}

		protected:
			virtual void update_system() override
			{
				enumerate([this](image_component* img) { draw(img); });
			}

		public:
			auto make_sprite(unsigned int glyph)-> decltype(make())
			{
				auto sprite = make();
				sprite->glyph = glyph;
				return sprite;
			}

		private:
			void draw(image_component* img)
			{
				m_canvas->write({ 0, 0 }, img->glyph);
			}
		};
	}
}

#endif