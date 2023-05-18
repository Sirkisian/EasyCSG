#pragma once
//-----
#include <vector>
#include<string>
#include <gl/glew.h>
#include <tchar.h>
#include "macros.hpp"
//----
//---
//--
class ShaderProgram
{
	public:
		static inline GLint setBool(GLuint shaderProgram, std::basic_string<GLchar> variable, GLboolean value)
		{
			return ShaderProgram::setInteger(shaderProgram, variable, value);
		}

		static GLint setInteger(GLuint shaderProgram, std::basic_string<GLchar> variable, GLint value);

		template <size_t size> 
		static GLint setVector(GLuint shaderProgram, std::basic_string<GLchar> variable, const std::array<GLfloat, size>* vector)
		{
			GLint location = glGetUniformLocation(shaderProgram, variable.data());

			if(location >= 0 && vector != nullptr)
			{
				if(size == 3)
				{
					glUniform3fv(location, 1, vector->data());
				}
				else if(size == 4)
				{
					glUniform4fv(location, 1, vector->data());
				}
			}

			return location;
		}

		static GLint setMatrix(GLuint shaderProgram, std::basic_string<GLchar> variable, _IN_(std::vector<GLfloat> & matrix));

		static GLvoid replaceShaderCode(_INOUT_(std::basic_string<GLchar> & code), _IN_(std::basic_string<GLchar> & from), _IN_(std::basic_string<GLchar> & to));

		ShaderProgram();
		~ShaderProgram();

		GLboolean addShaders(_IN_(std::basic_string<GLchar> & codeVertex), _IN_(std::basic_string<GLchar> & codeFragment));

		const GLuint & useShaderProgram();

		inline const GLuint & getShaderProgram()
		{
			return this->program;
		}

	private:
		GLuint createShader(GLenum shaderType, _IN_(std::basic_string<GLchar> & code));

		GLuint program;
};