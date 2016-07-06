// name: program.h
// type: c++ header
// desc: struct declaration
// auth: is0urce

// program scruct for opengl

#ifndef PX_SHELL_PROGRAM_H
#define PX_SHELL_PROGRAM_H

#include <gl\gl.h>
#include <gl\glu.h>

#include <px/shell/glsl.h>

#include <vector>
#include <memory>
#include <functional>

namespace px
{
	namespace shell
	{
		struct program
		{
		private:
			GLint m_id;
			bool m_init;
			std::function<void()> m_fn;

		public:
			program() : m_init(false), m_id(0)
			{
			}
			program(const std::string& path)
			{
				m_id = glsl::link(path);
				m_init = true;
			}
			~program()
			{
				if (m_init)
				{
					glDeleteProgram(m_id);
				}
			}
			program(const program&) = delete;
			program(program&& other) : program()
			{
				swap(other);
			}
			program& operator=(const program&) = delete;
			program& operator=(program&& other)
			{
				swap(other);
				return *this;
			}

			void swap(program& other)
			{
				std::swap(m_id, other.m_id);
				std::swap(m_init, other.m_init);
				std::swap(m_fn, other.m_fn);
			}

			void activate() const
			{
				glUseProgram(m_id);
			}
			GLint uniform(const char* c_str)
			{
				return glGetUniformLocation(m_id, c_str);
			}
			void static uniform(GLint uniform, GLint int_value)
			{
				glUniform1i(uniform, int_value);
			}
			void static uniform(GLint uniform, GLfloat float_value)
			{
				glUniform1f(uniform, float_value);
			}
			void static uniform(GLint uniform, GLfloat f0, GLfloat f1)
			{
				glUniform2f(uniform, f0, f1);
			}
			void prepare(std::function<void()> fn)
			{
				m_fn = fn;
			}
			GLint id() const
			{
				return m_id;
			}
			void use() const
			{
				use(true);
			}
			void use(bool prepare) const
			{
				activate();
				if (prepare && m_fn)
				{
					m_fn();
				}
			}

			// aux
			void uniform(const char* c_str, GLint int_value)
			{
				uniform(uniform(c_str), int_value);
			}
			void uniform(const char* c_str, GLfloat f0, GLfloat f1)
			{
				uniform(uniform(c_str), f0, f1);
			}
		};
	}
}

#endif