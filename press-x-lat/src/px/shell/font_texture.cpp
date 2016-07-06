// name: "font_texture.cpp"
// type: c++
// desc: class implementation
// auth: is0urce

#include "font_texture.h"

#include <px/shell/font.h>

#include <stdexcept>

namespace
{
	const int mipmaps_total = 8;
}

namespace px
{
	namespace shell
	{
		font_texture::font_texture()
		{
		}
		font_texture::font_texture(std::shared_ptr<font> font_ptr)
		{
			init(font_ptr);
		}
		font_texture::font_texture(std::unique_ptr<font> font_ptr)
		{
			init(std::shared_ptr<font>(font_ptr.release()));
		}
		font_texture::font_texture(font* font_ptr)
		{
			init(std::shared_ptr<font>(font_ptr, [](font* f){}));
		}

		// copy constructor
		font_texture::font_texture(const font_texture& other)
		{
			init(other.m_font);
		}

		// move constructor
		font_texture::font_texture(font_texture&& other)
			: font_texture()
		{
			this->swap(other);
		}

		font_texture::~font_texture()
		{
			release();
		}

		font_texture& font_texture::operator=(font_texture other)
		{
			this->swap(other);
			return *this;
		}
		font* font_texture::operator->()
		{
			return m_font.get();
		}

		void font_texture::swap(font_texture &other)
		{
			std::swap(this->m_font, other.m_font);
			std::swap(this->m_texture, other.m_texture);
			std::swap(this->m_version, other.m_version);
		}

		void font_texture::init(std::shared_ptr<font> font_ptr)
		{
			release();
			if (!font_ptr) throw std::runtime_error("font_texture.ctor(..) font is null");

			glGenTextures(1, &m_texture);

			m_font = font_ptr;
			update(true);
		}
		void font_texture::release()
		{
			if (m_font)
			{
				glDeleteTextures(1, &m_texture);
			}
		}
		void font_texture::update()
		{
			update(false);
		}
		void font_texture::update(bool force)
		{
			if (m_font)
			{
				auto version = m_font->version();
				if (force || version != m_version)
				{
					m_version = version;

					const auto &ui_texture = m_font->texture();
					glBindTexture(GL_TEXTURE_2D, m_texture);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, mipmaps_total);
					glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, ui_texture.width, ui_texture.height, 0, GL_RED, GL_UNSIGNED_BYTE, ui_texture.data);
					glEnable(GL_TEXTURE_2D);
					glGenerateMipmap(GL_TEXTURE_2D);
				}
			}
		}

		font_texture::texture_t font_texture::last_texture() const
		{
			return m_texture;
		}
		font_texture::texture_t font_texture::texture_id()
		{
			update();
			return m_texture;
		}
		void font_texture::bind(unsigned int texure0_plus)
		{
			glActiveTexture(GL_TEXTURE0 + texure0_plus);
			glBindTexture(GL_TEXTURE_2D, texture_id());
		}
	}
}

