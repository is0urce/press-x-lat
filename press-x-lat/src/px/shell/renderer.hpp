// name: renderer.h
// type: c++ header file
// desc: opengl rendering
// auth: is0urce

// extends ogl with drawing functions

#ifndef PX_SHELL_RENDERER_H
#define PX_SHELL_RENDERER_H

#include <px/shell/vao.h>
#include <px/shell/program.h>
#include <px/shell/texture.h>
#include <px/shell/font.hpp>
#include <px/shell/font_texture.h>
#include <px/shell/canvas.hpp>

#include <px/common/point.hpp>
#include <px/common/vector.hpp>

#include <memory>

namespace px
{
	namespace shell
	{
		class opengl;
		class renderer
		{
		public:
			typedef double time_t;

		public:
			const static unsigned int ui_cell_width = 16;
			const static unsigned int ui_cell_height = 16;

		private:
			opengl* m_opengl;
			int m_width, m_height;
			double m_aspect;
			double m_scale;
			unsigned int m_pixel_scale;

			vector2 m_camera; // camera offset

			// ui rendering
			struct ui_draw
			{
			public:
				font_texture font_texture;
				// buffers cashed due in general constant canvas size
				int width = 0;
				int height = 0;
				struct background_draw
				{
					vao vao;
					program shader;
					std::vector<GLfloat> vertices;
					std::vector<GLfloat> colors;
				} bg;
				struct symbol_draw
				{
					vao vao;
					program shader;
					std::vector<GLfloat> vertices;
					std::vector<GLfloat> colors;
					std::vector<GLfloat> texture;
				} text;
				std::vector<GLuint> indices; // indices are shared
				float scale_x, scale_y, offset_x, offset_y; // uniform values
			} m_ui;

		public:
			renderer(opengl *opengl);
			virtual ~renderer();

		public:
			void render(time_t time, const canvas& cnv);
			void scale(double delta);
			void pixel_clip(unsigned int ppu, unsigned int multiptier);
			void canvas_size(int& w, int &h);
			point2 renderer::translate_canvas(const point2 &screen) const;

		private:
			void draw_canvas(const canvas& cnv);
		};
	}
}

#endif