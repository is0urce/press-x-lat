// name: vao.h
// type: c++ header
// desc: struct declaration
// auth: is0urce

// vertex array scruct for opengl

#ifndef PX_SHELL_VAO_H
#define PX_SHELL_VAO_H

#include <gl\glew.h>
#include <gl\gl.h>
#include <gl\glu.h>

#include <vector>
#include <memory>

namespace px
{
	namespace shell
	{
		struct vao
		{
		private:
			bool m_init;
			GLuint m_vao;
			unsigned int m_num; // number of attributes
			std::vector<unsigned int> m_depth; // attribute depth
			std::vector<GLuint> m_buffers;

			GLuint m_indices;
			unsigned int m_length; // size of index array

		public:
			vao() : m_init(false), m_num(0), m_length(0)
			{
			}
			vao(std::vector<unsigned int> depths) : m_init(false), m_num(0), m_length(0)
			{
				init(depths);
			}
			vao(const vao&) = delete;
			vao& operator=(const vao&) = delete;
			vao(vao&& other) : vao()
			{
				swap(other);
			}
			vao& operator=(vao&& other)
			{
				swap(other);
				return *this;
			}
			~vao()
			{
				release();
			}


		public:
			void swap(vao& other)
			{
				std::swap(m_init, other.m_init);
				std::swap(m_vao, other.m_vao);
				std::swap(m_num, other.m_num);
				std::swap(m_depth, other.m_depth);
				std::swap(m_buffers, other.m_buffers);
				std::swap(m_indices, other.m_indices);
				std::swap(m_length, other.m_length);
			}

			// count - number of buffers
			// depth - array of buffer element size
			void init(std::vector<unsigned int> depths)
			{
				if (m_init)
				{
					release();
				}
				m_num = depths.size();
				if (m_num == 0) throw std::logic_error("vao::init - count == 0");

				m_depth = depths;
				for (unsigned int i = 0; i < m_num; ++i)
				{
					if (depths[i] == 0) throw std::runtime_error("vao::init - depth == 0");
				}
				m_buffers.resize(m_num, 0);

				glGenBuffers(1, &m_indices);
				glGenBuffers(m_num, &m_buffers[0]);
				glGenVertexArrays(1, &m_vao);
				glBindVertexArray(m_vao);
				for (unsigned int i = 0; i < m_num; ++i)
				{
					glEnableVertexAttribArray(i);
				}
				for (unsigned int i = 0; i < m_num; ++i)
				{
					glBindBuffer(GL_ARRAY_BUFFER, m_buffers[i]);
					glVertexAttribPointer(i, depths[i], GL_FLOAT, GL_FALSE, 0, 0);
				}
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indices);

				m_init = true;
			}

			unsigned int depth(unsigned int attribute) const
			{
				return m_depth.at(attribute);
			}
			inline unsigned int length() const
			{
				return m_length;
			}
			inline unsigned int attributes() const
			{
				return m_num;
			}

			void release()
			{
				if (m_init)
				{
					glDeleteVertexArrays(1, &m_vao);
					glDeleteBuffers(1, &m_indices);
					glDeleteBuffers(m_num, &m_buffers[0]);
				}
				m_init = false;
				m_num = 0;
				m_length = 0;
				m_depth.clear();
				m_buffers.clear();
			}

			// points - number of points in values
			// length - number of indices in index_values
			void fill_attributes(unsigned int attribute_index, unsigned int points, const GLfloat* attribute_values)
			{
				if (!attribute_values) return;
				glBindBuffer(GL_ARRAY_BUFFER, m_buffers[attribute_index]);
				glBufferData(GL_ARRAY_BUFFER, sizeof(attribute_values[0]) * points * m_depth[attribute_index], attribute_values, GL_STATIC_DRAW);
			}
			void fill_attributes(unsigned int attribute_index, const std::vector<GLfloat> &attribute_values)
			{
				unsigned int points = attribute_values.size();
				if (points == 0) return;
				glBindBuffer(GL_ARRAY_BUFFER, m_buffers[attribute_index]);
				glBufferData(GL_ARRAY_BUFFER, sizeof(attribute_values[0]) * points * m_depth[attribute_index], &attribute_values[0], GL_STATIC_DRAW);
			}
			void fill_indices(unsigned int length, const GLuint* index_values)
			{
				m_length = length;
				if (index_values)
				{
					glBindVertexArray(m_vao);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indices);
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_values[0]) * m_length, index_values, GL_STATIC_DRAW);
				}
			}
			void fill_indices(const std::vector<GLuint> &index_values)
			{
				m_length = index_values.size();
				if (m_length > 0)
				{
					glBindVertexArray(m_vao);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indices);
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_values[0]) * m_length, &index_values[0], GL_STATIC_DRAW);
				}
			}
			void clear()
			{
				fill_indices(0, nullptr);
			}

			void draw(GLuint element_type) const
			{
				if (!m_init) throw std::runtime_error("px::shell::vao::draw - not initialized");

				if (m_length > 0)
				{
					glBindVertexArray(m_vao);
					glDrawElements(element_type, m_length, GL_UNSIGNED_INT, 0);
				}
			}
			void draw() const
			{
				draw(GL_TRIANGLES);
			}
		};
	}
}

#endif