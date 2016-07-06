// name: "font_texture.h"
// type: c++
// desc: class header
// auth: is0urce

// opengl texture with font

#ifndef PX_SHELL_FONT_TEXTURE_H
#define PX_SHELL_FONT_TEXTURE_H

#include <gl/glew.h>

#include <memory>

namespace px
{
	namespace shell
	{
		class font;
		class font_texture final
		{
			typedef GLuint texture_t;

		private:
			texture_t m_texture;
			std::shared_ptr<font> m_font;
			unsigned int m_version;

		public:
			~font_texture();
			font_texture();
			font_texture(std::shared_ptr<font>);
			font_texture(std::unique_ptr<font>);
			font_texture(font*);
			font_texture(const font_texture&);
			font_texture(font_texture&&);

		public:
			void init(std::shared_ptr<font>);
			void release();
			void swap(font_texture &other);
			void update();
			void update(bool force);
			texture_t last_texture() const;
			texture_t texture_id();
			void bind(unsigned int texure0_plus);

			font_texture& operator=(font_texture other);
			font* operator->();
		};
	}
}

#endif