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
#include <px/shell/font_texture.h>
#include <px/shell/sprite_sheet.hpp>

#include <px/shell/perception.h>
#include <px/shell/sprite_manager.hpp>

#include <px/ui/canvas.h>

#include <px/point.hpp>
#include <px/vector.hpp>

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
			const static unsigned int ui_cell_height = 20;

		private:
			opengl* m_opengl;
			int m_width, m_height;
			double m_aspect;
			double m_scale;
			unsigned int m_pixel_scale;
			ui::canvas m_canvas;
			perception m_perception;

			vector m_camera; // camera offset
			sprite_sheet m_sheet;

			// ui rendering
			struct ui_draw
			{
			public:
				// buffers cashed due constant canvas size
				int width;
				int height;
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
					font_texture font;
				} text;
				std::vector<GLuint> indices; // indices are shared
				float scale_x, scale_y, offset_x, offset_y; // uniform values
			} m_ui;

			// terrain tile sprites
			struct tile_draw
			{
			public:
				texture sheet;
				vao vao;
				std::vector<GLfloat> vertices;
				std::vector<GLfloat> colors; // lights and tints
				std::vector<GLfloat> textcoords;
				std::vector<GLuint> indices;
				program shader;
			} m_tile;
			// unit sprites
			struct sprite_draw
			{
			public:
				sprite_manager manager;
				vao vao;
				unsigned int max = 0;
				std::vector<GLfloat> vertices, colors, texture;
				std::vector<GLuint> indices;
				program shader;
			} m_sprite;

		public:
			renderer(opengl *opengl);
			virtual ~renderer();

		public:
			void render(time_t time);
			void scale(double delta);
			void pixel_clip(unsigned int ppu, unsigned int multiptier);
			ui::canvas& canvas();
			sprite_manager* sprite_manager() { return &m_sprite.manager; }
			sprite_sheet* sprite_sheet() { return &m_sheet; }
			perception* perception() { return &m_perception; }

		private:
			void draw_canvas(const ui::canvas& gui);
			void draw_sprites(time_t time);
			void draw_terrain(time_t time);
		};
	}
}

#endif