// name: "font.h"
// type: c++
// desc: class header
// auth: is0urce

// fonts management using freetype

#ifndef PX_SHELL_FONT_H
#define PX_SHELL_FONT_H

#include <ft2build.h>
#include FT_FREETYPE_H

#include <memory>
#include <map>

namespace px
{
	namespace shell
	{
		struct glyph
		{
			double top, bottom, left, right; // texture coordinates
			double width, height; // [0, 1] size
			double advance;
			unsigned int pixel_width;
			unsigned int pixel_height;

			// baseline offsets
			double vertical;
			double horisontal;
			unsigned int pixel_left; // bitmap_top
			unsigned int pixel_top; // bitmap_left

			glyph() : width(0), height(0) {};
		};

		class font
		{
		public:
			struct font_texture
			{
				unsigned int width;
				unsigned int height;
				const char* data;
			};

		private:
			static unsigned int m_count;
			static FT_Library m_lib;
			FT_Face m_face;
			unsigned int m_size;

			std::map<unsigned int, std::unique_ptr<glyph>> m_letters;
			std::unique_ptr<std::unique_ptr<glyph>[]> m_base;
			std::unique_ptr<char[]> m_atlas;
			unsigned int m_width, m_height; // atlas size
			unsigned int m_penx, m_peny, m_next_line;
			font_texture m_texture;
			bool m_dirty; // is updated?
			unsigned int m_version;

		public:
			font(const char* name, unsigned int size);
			~font();
		private:
			font(const font&); // disable copy

		private:
			std::unique_ptr<glyph> raster(unsigned int uplus);

		public:
			bool updated() const;
			void updated(bool is_updated);
			unsigned int version() const;
			const font::font_texture& texture() const;
			unsigned int size() const;

			void include(unsigned int uplus);
			void include(unsigned int uplus, unsigned int target_index);

			double kerning(unsigned int left, unsigned int right) const;
			const glyph& operator[](unsigned int unicode_plus);
			const glyph& at(unsigned int unicode_plus);
		};
	}
}

#endif