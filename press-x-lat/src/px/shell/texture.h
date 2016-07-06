// name: "texture.h"
// type: c++ header
// desc: class
// auth: is0urce

// opengl texture

#ifndef PX_SHELL_TEXTURE_H
#define PX_SHELL_TEXTURE_H

#include <gl/glew.h>

#include <memory>

namespace px
{
	namespace shell
	{
		class texture
		{
			typedef GLuint texture_t;

		private:
			texture_t m_texture;

		public:
			~texture()
			{
				glDeleteTextures(1, &m_texture);
			}
			texture()
			{
				glGenTextures(1, &m_texture);
			}
			texture(const texture&) = delete;

		public:
			texture_t texture_id() const
			{
				return m_texture;
			}
			void bind(unsigned int texure0_plus)
			{
				glActiveTexture(GL_TEXTURE0 + texure0_plus);
				glBindTexture(GL_TEXTURE_2D, m_texture);
			}
			void init(unsigned int width, unsigned int height, unsigned int mipmaps, const void* raw_pixels)
			{
				glBindTexture(GL_TEXTURE_2D, m_texture);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, mipmaps);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, raw_pixels);
				glEnable(GL_TEXTURE_2D);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
		};
	}
}

#endif