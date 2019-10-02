#pragma once
//-----
#include <string>
#include "shaderprogram.hpp"
//----
//---
//--
namespace VertexShader
{
	enum class TYPE : unsigned char {FIXEDV, SHADERV};

	const std::basic_string<GLchar> codeVertex( 
	"#version 120\n"

	"attribute vec3 position;" //from vertex
	"attribute vec3 normal;"
	"attribute vec2 texture;"

	"uniform mat4 viewProjection = mat4(1.0);"
	"uniform mat4 model = mat4(1.0);"
	"uniform mat3 transposeInverseModel = mat3(1.0);"

	"varying vec3 fragPosition;"
	"varying vec3 fragNormal;"
	"varying vec2 fragTexture;"

	"void main()"
	"{"
	"	vec4 position4 = vec4(position, 1.0);"
	"	"
	"	fragPosition = vec3(model * position4);"
	"	"
	"	fragNormal = transposeInverseModel * normal;"
	"	"
	"	fragTexture = texture;"
	"	"
	"	gl_Position = #REPLACE_POSITION#;"
	"}");

	static std::basic_string<GLchar> getVertexShader(VertexShader::TYPE type)
	{
		std::basic_string<GLchar> vertexCode = VertexShader::codeVertex;

		switch(type)
		{
			case VertexShader::TYPE::SHADERV:
				{
					ShaderProgram::replaceShaderCode(vertexCode, std::basic_string<GLchar>("#REPLACE_POSITION#"), std::basic_string<GLchar>("viewProjection * vec4(fragPosition, 1.0)"));
				}
				break;

			case VertexShader::TYPE::FIXEDV:
				{
					ShaderProgram::replaceShaderCode(vertexCode, std::basic_string<GLchar>("#REPLACE_POSITION#"), std::basic_string<GLchar>("gl_ModelViewProjectionMatrix * position4")); // to avoid Z fighting after object transfrormation (different rounding in fixed pipeline and shaders);
				}
				break;
		}

		//ShaderProgram::replaceShaderCode(vertexCode, std::basic_string<GLchar>(";"), std::basic_string<GLchar>(";\n"));

		return vertexCode;
	}

	const std::basic_string<GLchar> codeFontVertex(
	"#version 120\n"

	"attribute vec4 vertex;"

	"uniform mat4 projection = mat4(1.0);"

	"varying vec2 textPosition;"

	"void main()"
	"{"
	"	textPosition = vertex.zw;"
	"	"
	"	gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);"
	"}");
}