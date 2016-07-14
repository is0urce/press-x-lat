// name: renderer.cpp
// type: c++ class implementation
// desc: opengl rendering
// auth: is0urce

#include "renderer.hpp"

#include <px/shell/opengl.h>
#include <px/shell/font.h>

#include <px/common/color.hpp>

#include <memory>
#include <stdexcept>
#include <vector>
#include <cmath>
#include <functional>

namespace px
{
	namespace shell
	{
		namespace
		{
			const unsigned int quad = 4; // four vertices for quad
			const unsigned int strip = 6; // six indices for 2-triangles
			const unsigned int texture_depth = 2;
			const unsigned int vertex_depth = 4;
			const unsigned int color_depth = 4;

			const double zoom_exp = 1.0 / 1000;
			const double zoom_min = 0.01;
			const double zoom_max = 10000;

			const char* font_name = "PragmataPro.ttf";
			const unsigned int font_size = renderer::ui_cell_height;

			//const char* texture_path = "textures/compiled.png";
			//const char* texture_desc = "textures/compiled.json";

			//void fill_vertex(const point2 &position, GLfloat* dest)
			//{
			//	GLfloat x = (GLfloat)position.x();
			//	GLfloat y = (GLfloat)position.y();
			//	GLfloat dx = (GLfloat)(position.x() + 1);
			//	GLfloat dy = (GLfloat)(position.y() + 1);
			//	x -= 0.5f;
			//	y -= 0.5f;
			//	dx -= 0.5f;
			//	dy -= 0.5f;

			//	dest[0] = x;
			//	dest[1] = y;
			//	dest[2] = 0;
			//	dest[3] = 1;
			//	dest[4] = x;
			//	dest[5] = dy;
			//	dest[6] = 0;
			//	dest[7] = 1;
			//	dest[8] = dx;
			//	dest[9] = dy;
			//	dest[10] = 0;
			//	dest[11] = 1;
			//	dest[12] = dx;
			//	dest[13] = y;
			//	dest[14] = 0;
			//	dest[15] = 1;
			//}
			//void fill_vertex(const vector2 &position, const vector2 &quad_range, GLfloat *dest)
			//{
			//	(position + vector2(-0.5, -0.5) * quad_range).write(dest + 0 * vertex_depth);
			//	(position + vector2(-0.5, 0.5) * quad_range).write(dest + 1 * vertex_depth);
			//	(position + vector2(0.5, 0.5) * quad_range).write(dest + 2 * vertex_depth);
			//	(position + vector2(0.5, -0.5) * quad_range).write(dest + 3 * vertex_depth);
			//}
			void fill_color(const color &c, GLfloat *dest)
			{
				c.write(dest, quad);
			}
			void fill_texture(GLfloat left, GLfloat bottom, GLfloat right, GLfloat top, GLfloat *dest)
			{
				dest[0] = left;
				dest[1] = bottom;
				dest[2] = left;
				dest[3] = top;
				dest[4] = right;
				dest[5] = top;
				dest[6] = right;
				dest[7] = bottom;
			}
			inline void fill_index(unsigned int num, GLuint *dest)
			{
				unsigned int index_offset = 0;
				for (unsigned int i = 0; i < num; ++i)
				{
					dest[index_offset + 0] = i * quad + 0;
					dest[index_offset + 1] = i * quad + 2;
					dest[index_offset + 2] = i * quad + 1;
					dest[index_offset + 3] = i * quad + 0;
					dest[index_offset + 4] = i * quad + 3;
					dest[index_offset + 5] = i * quad + 2;
					index_offset += strip;
				}
			}
			//template<typename _T>
			//const _T* to_pointer(const std::vector<_T>& a)
			//{
			//	return (a.size() == 0) ? nullptr : &a[0];
			//}
			//template<typename _T>
			//_T* to_pointer(std::vector<_T>& a)
			//{
			//	return (a.size() == 0) ? nullptr : &a[0];
			//}
		}
		renderer::renderer(opengl *opengl)
			: m_opengl(opengl)
			, m_scale(0.05f), m_pixel_scale(2)
		{
			if (!opengl) throw std::runtime_error("renderer::renderer(opengl* opengl) - opengl is null");

			// opengl setup
			glEnable(GL_TEXTURE_2D);
			glClampColor(GL_CLAMP_VERTEX_COLOR, GL_FALSE); // don't clamp hdr
			glClampColor(GL_CLAMP_READ_COLOR, GL_FALSE);
			glClampColor(GL_CLAMP_FRAGMENT_COLOR, GL_FALSE);

			m_ui.text.vao = vao({ 2, 4, 2 });
			m_ui.text.shader = program("console_text");
			m_ui.text.font = std::make_unique<font>(font_name, font_size);
			m_ui.text.shader.activate();
			m_ui.text.shader.uniform("font", 0);
			m_ui.text.u_scale = m_ui.text.shader.uniform("scale");
			m_ui.text.u_offset = m_ui.text.shader.uniform("offset");
			m_ui.text.shader.prepare([this]()
			{
				program::uniform(m_ui.text.u_scale, (GLfloat)m_ui.scale_x, (GLfloat)m_ui.scale_y);
				program::uniform(m_ui.text.u_offset, (GLfloat)m_ui.offset_x, (GLfloat)m_ui.offset_y);
				m_ui.text.font.bind(0);
			});

#if _DEBUG
			GLenum err = GL_NO_ERROR;
			while ((err = glGetError()) != GL_NO_ERROR)
			{
				throw std::runtime_error("renderer::renderer() - OpenGL error #" + std::to_string(err));
			}
#endif
		}
		renderer::~renderer()
		{
		}

		void renderer::render(time_t time, const canvas& cnv)
		{
			m_opengl->update(m_width, m_height);

			m_aspect = m_width;
			m_aspect /= m_height;

			glViewport(0, 0, (GLsizei)m_width, (GLsizei)m_height);
			glClearColor(0, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			draw_canvas(cnv);

			m_opengl->swap();

#if _DEBUG
			GLenum err = GL_NO_ERROR;
			while ((err = glGetError()) != GL_NO_ERROR)
			{
				throw std::runtime_error("renderer::render(..) - OpenGL error");
			}
#endif
		}
		
		void renderer::draw_canvas(const canvas& cnv)
		{
			//// ui
			int w = cnv.width();
			int h = cnv.height();
			if (w <= 0 || h <= 0) return;
			int size = w * h;

			//// update cashed buffers if sizes have been changed
			if (w != m_ui.width || h != m_ui.height)
			{
				m_ui.width = w;
				m_ui.height = h;

				// update buffer sizes
				m_ui.indices.resize(size * strip);

				m_ui.text.vertices.resize(size * 2 * quad);
				m_ui.text.colors.resize(size * 4 * quad);
				m_ui.text.texture.resize(size * 2 * quad);

				fill_index(size, &m_ui.indices[0]);

				// fill cashed parts
				m_ui.text.vao.fill_indices(m_ui.indices);
			}

			// fill buffers
			unsigned int color_offset = 0;
			unsigned int vertex_offset = 0;
			unsigned int texture_offset = 0;
			for (int j = 0; j < h; ++j)
			{
				for (int i = 0; i < w; ++i)
				{
					auto &symbol = cnv[{i, j}];
					auto &g = m_ui.text.font->at(symbol.code == 0 ? '?' : symbol.code);

					// vertices
					int x = i * ui_cell_width + ui_cell_width / 2 - g.pixel_width / 2;
					int xd = x + g.pixel_width;
					int y = (h - j - 1) * ui_cell_height - g.pixel_height + g.pixel_top;
					int yd = y + g.pixel_height;

					m_ui.text.vertices[vertex_offset + 0] = (GLfloat)x;
					m_ui.text.vertices[vertex_offset + 1] = (GLfloat)y;
					m_ui.text.vertices[vertex_offset + 2] = (GLfloat)x;
					m_ui.text.vertices[vertex_offset + 3] = (GLfloat)yd;
					m_ui.text.vertices[vertex_offset + 4] = (GLfloat)xd;
					m_ui.text.vertices[vertex_offset + 5] = (GLfloat)yd;
					m_ui.text.vertices[vertex_offset + 6] = (GLfloat)xd;
					m_ui.text.vertices[vertex_offset + 7] = (GLfloat)y;

					// colors
					fill_color(symbol.front, &m_ui.text.colors[color_offset]);

					// textures
					fill_texture((GLfloat)g.left, (GLfloat)g.bottom, (GLfloat)g.right, (GLfloat)g.top, &m_ui.text.texture[texture_offset]);

					vertex_offset += 2 * quad;
					color_offset += color_depth * quad;
					texture_offset += texture_depth * quad;
				}
			}

			// setup drawing
			glViewport(0, 0, (GLsizei)m_width, (GLsizei)m_height);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			// uniforms source preparation 
			m_ui.scale_x = 2.0f / m_width;
			m_ui.scale_y = 2.0f / m_height;
			m_ui.offset_x = (float)(m_width % ui_cell_width / 2);
			m_ui.offset_y = (float)(m_height % ui_cell_height / 2);

			// text
			m_ui.text.shader.use();
			m_ui.text.vao.fill_attributes(0, size * quad, &m_ui.text.vertices[0]);
			m_ui.text.vao.fill_attributes(1, size * quad, &m_ui.text.colors[0]);
			m_ui.text.vao.fill_attributes(2, size * quad, &m_ui.text.texture[0]);
			m_ui.text.vao.fill_indices(m_ui.indices);
			m_ui.text.vao.draw();
		}

		void renderer::scale(double pan)
		{
			m_scale *= 1 + pan * zoom_exp;
			m_scale = (std::max)(zoom_min, (std::min)(m_scale, zoom_max)); // clamp

			m_pixel_scale = pan > 0 ? m_pixel_scale << 1 : m_pixel_scale >> 1;
			m_pixel_scale = (std::max)(1u, (std::min)(m_pixel_scale, 16u)); // clamp
		}
		void renderer::pixel_clip(unsigned int ppu, unsigned int multiplier)
		{
			m_scale = ppu * multiplier * 2; // 2 is size of opengl screen range (-1,1)
			m_scale /= m_width;
		}

		void renderer::canvas_size(int& w, int &h)
		{
			m_opengl->update(w, h);
			w = (std::max<int>)(1, w / ui_cell_width);
			h = (std::max<int>)(1, h / ui_cell_height);
		}
	}
}