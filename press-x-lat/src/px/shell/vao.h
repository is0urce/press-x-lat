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
			std::unique_ptr<unsigned int[]> m_depth; // attribute depth
			std::unique_ptr<GLuint[]> m_buffers;

			GLuint m_indices;
			unsigned int m_length; // size of index array

		public:
			vao() : m_init(false), m_num(0), m_length(0)
			{
			}
			vao(const std::vector<unsigned int> &depths) : m_init(false), m_num(0), m_length(0)
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
			inline void init(unsigned int count, const unsigned int* depths)
			{
				if (count == 0) throw std::logic_error("vao::init - count == 0");
				if (!depths) throw std::logic_error("vao::init - depths is null");

				if (m_init)
				{
					release();
				}

				m_num = count;
				m_depth.reset(new unsigned int[count]);
				for (unsigned int i = 0; i < count; ++i)
				{
					if (depths[i] == 0) throw std::runtime_error("vao::init - depth == 0");
					m_depth[i] = depths[i];
				}
				m_buffers.reset(new GLuint[count]);

				glGenBuffers(1, &m_indices);
				glGenBuffers(count, m_buffers.get());
				glGenVertexArrays(1, &m_vao);
				glBindVertexArray(m_vao);
				for (unsigned int i = 0; i < count; ++i)
				{
					glEnableVertexAttribArray(i);
				}
				for (unsigned int i = 0; i < count; ++i)
				{
					glBindBuffer(GL_ARRAY_BUFFER, m_buffers[i]);
					glVertexAttribPointer(i, depths[i], GL_FLOAT, GL_FALSE, 0, 0);
				}
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indices);

				m_init = true;
			}

			inline void init(const std::vector<unsigned int> &depths)
			{
				if (depths.empty()) throw std::exception("px::shell::vao::init(std::vector<unsigned int> &depths) - depth is empty");
				init(depths.size(), &depths[0]);
			}

			inline unsigned int depth(unsigned int attribute) const
			{
				if (attribute >= m_num) throw std::runtime_error("px::shell::vao::depth(unsigned int attribute) const - attribute out of range");
				return m_depth[attribute];
			}
			inline unsigned int length() const
			{
				return m_length;
			}
			inline unsigned int attributes() const
			{
				return m_num;
			}

			inline void release()
			{
				if (m_init)
				{
					glDeleteVertexArrays(1, &m_vao);
					glDeleteBuffers(1, &m_indices);
					glDeleteBuffers(m_num, m_buffers.get());
				}
				m_init = false;
				m_num = 0;
				m_length = 0;
				m_depth.reset();
				m_buffers.reset();
			}

			// points - number of points in values
			// length - number of indices in index_values
			inline void fill_attributes(unsigned int points, unsigned int attribute_index, GLfloat const* attribute_values)
			{
				glBindBuffer(GL_ARRAY_BUFFER, m_buffers[attribute_index]);
				glBufferData(GL_ARRAY_BUFFER, sizeof(attribute_values[0]) * points * m_depth[attribute_index], attribute_values, GL_STATIC_DRAW);
			}
			inline void fill_indices(unsigned int length, GLuint const* index_values)
			{
				m_length = length;
				if (index_values)
				{
					glBindVertexArray(m_vao);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indices);
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_values[0]) * length, index_values, GL_STATIC_DRAW);
				}
			}
			inline void clear()
			{
				fill_indices(0, nullptr);
			}

			inline void fill_attributes(unsigned int points, const GLfloat* const* attribute_values)
			{
				for (unsigned int i = 0; i < m_num; ++i)
				{
					if (attribute_values[i])
					{
						fill_attributes(points, i, attribute_values[i]);
					}
				}
			}
			inline void fill(unsigned int points, unsigned int length, const GLfloat* const* attribute_values, const GLuint* index_values)
			{
				if (!m_init) throw std::runtime_error("px::shell::vao::fill - not initialized");

				fill_indices(length, index_values);
				fill_attributes(points, attribute_values);
			}
			//inline void fill(unsigned int points, const std::vector<std::vector<GLfloat>*> &attribute_values, const std::vector<GLuint> &index_values)
			//{
			//	auto av_size = attribute_values.size();
			//	if (av_size == 0) throw std::runtime_error("px::shell::vao::fill - attribute values size = 0");
			//	if (av_size != m_num)  throw std::runtime_error("px::shell::vao::fill - attribute num missmatch");

			//	std::vector<GLfloat*> av(av_size);
			//	unsigned int c = 0;
			//	for (auto it = attribute_values.begin(), last = attribute_values.end(); it != last; ++it)
			//	{
			//		std::vector<GLfloat>* v = *it;
			//		av[c] = (v && v->size() > 0 ? &(*v)[0] : nullptr);
			//		++c;
			//	}
			//	fill(points, index_values.size(), &av[0], index_values.size() == 0 ? nullptr : &index_values[0]);
			//}

			inline void draw(GLuint element_type) const
			{
				if (!m_init) throw std::runtime_error("px::shell::vao::draw - not initialized");

				if (m_length > 0)
				{
					glBindVertexArray(m_vao);
					glDrawElements(element_type, m_length, GL_UNSIGNED_INT, 0);
				}
			}
			inline void draw() const
			{
				draw(GL_TRIANGLES);
			}

			inline void draw(int points, int length, GLfloat** values, GLuint* index_values)
			{
				fill(points, length, values, index_values);
				draw();
			}
		};
	}
}

#endif