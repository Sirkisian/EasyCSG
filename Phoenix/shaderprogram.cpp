#include "shaderprogram.hpp"

GLint ShaderProgram::setInteger(GLuint shaderProgram, std::basic_string<GLchar> variable, GLint value)
{
	GLint location = glGetUniformLocation(shaderProgram, variable.data());

	if(location >= 0)
	{
		glUniform1i(location, value);
	}

	return location;
}

GLint ShaderProgram::setMatrix(GLuint shaderProgram, std::basic_string<GLchar> variable, _IN_(std::vector<GLfloat> & matrix))
{
	GLint location = glGetUniformLocation(shaderProgram, variable.data());

	if(location >= 0)
	{
		size_t size = matrix.size();

		if(size == 9)
		{
			glUniformMatrix3fv(location, 1, GL_FALSE, matrix.data());
		}
		else if(size == 16)
		{
			glUniformMatrix4fv(location, 1, GL_FALSE, matrix.data());
		}
	}

	return location;
}

ShaderProgram::ShaderProgram():
program(0)
{
}

ShaderProgram::~ShaderProgram()
{
	if(this->program > 0)
	{
		glDeleteProgram(this->program);
	}
}

GLvoid ShaderProgram::replaceShaderCode(_INOUT_(std::basic_string<GLchar> & code), _IN_(std::basic_string<GLchar> & from), _IN_(std::basic_string<GLchar> & to))
{
	std::basic_string<GLchar> substringLeft;
	std::basic_string<GLchar> substringRight;

	size_t sizeFrom = from.size();
	size_t sizeTo = to.size();

	if(sizeFrom > 0)
	{
		size_t position = code.find(from);

		while(position != std::string::npos)
		{
			substringLeft = code.substr(0, position);
			substringRight = code.substr(position + sizeFrom);

			code = substringLeft + to + substringRight;

			position = code.find(from, substringLeft.size() + sizeTo);
		}
	}
}

GLboolean ShaderProgram::addShaders(_IN_(std::basic_string<GLchar> & codeVertex), _IN_(std::basic_string<GLchar> & codeFragment))
{
	GLint result = GL_FALSE;
	GLuint vertexShader = 0, fragmentShader = 0;

	if(!codeVertex.empty())
	{
		vertexShader = this->createShader(GL_VERTEX_SHADER, codeVertex);
	}

	if(!codeFragment.empty())
	{
		fragmentShader = this->createShader(GL_FRAGMENT_SHADER, codeFragment);
	}

	if(vertexShader > 0 && fragmentShader > 0)
	{
		this->program = glCreateProgram();

		if(this->program > 0)
		{
			glAttachShader(this->program, vertexShader);
			glAttachShader(this->program, fragmentShader);

			glLinkProgram(this->program);

			glGetProgramiv(this->program, GL_LINK_STATUS, &result);
		}

		if(result != GL_TRUE)
		{
			if(this->program > 0)
			{
				glDeleteProgram(this->program);
				this->program = 0;
			}
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	return result;
}

GLuint ShaderProgram::createShader(GLenum shaderType, _IN_(std::basic_string<GLchar> & code))
{
	GLuint shader = glCreateShader(shaderType);

	if(shader > 0)
	{
		GLint result = 0;

		const GLchar* const constCode = code.data();
		glShaderSource(shader, 1, &constCode, nullptr);

		glCompileShader(shader);

		glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

		if(result != GL_TRUE)
		{
			GLint size = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &size);

			std::basic_string<GLchar> error;
			error.resize(size);

			glGetShaderInfoLog(shader, size, &size, &error.front());

			glDeleteShader(shader);
			shader = 0;
		}
	}

	return shader;
}

const GLuint & ShaderProgram::useShaderProgram()
{
	glUseProgram(this->program);

	return this->program;
}