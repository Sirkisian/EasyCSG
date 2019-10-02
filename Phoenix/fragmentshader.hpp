#pragma once
//-----
#include <string>
#include "shaderprogram.hpp"
//----
//---
//--
namespace FragmentShader
{
	enum class TYPE : unsigned char {FIXEDF, SHADERF};

	const std::basic_string<GLchar> codeFragment(
	"#version 120\n"

	"uniform bool lighting = false;"
	"uniform bool texture = false;"

	"uniform vec4 color = vec4(0.0, 0.0, 0.0, 1.0);"

	"struct Light {"
	"vec3 position;"
	"vec4 ambient;"
	"vec4 diffuse;"
	"vec4 specular;"
	"};"

	"struct Material {"
	"vec4 ambient;"
	"vec4 diffuse;"
	"vec4 specular;"
	"vec4 emissive;"
	"};"

	"uniform Light light = Light(vec3(0.0, 0.0, 0.0), vec4(0.0, 0.0, 0.0, 1.0), vec4(0.0, 0.0, 0.0, 1.0), vec4(0.0, 0.0, 0.0, 1.0));"
	"uniform Material material = Material(vec4(0.0, 0.0, 0.0, 1.0), vec4(0.0, 0.0, 0.0, 1.0), vec4(0.0, 0.0, 0.0, 1.0), vec4(0.0, 0.0, 0.0, 1.0));"

	"uniform vec3 viewPosition = vec3(0.0, 0.0, 0.0);"

	"uniform sampler2D image;" //dont need to be set

	"varying vec3 fragPosition;"
	"varying vec3 fragNormal;"
	"varying vec2 fragTexture;"

	"void main()"
	"{"
	"	#REPLACE_LIGHT_IF#"
	"		vec4 ambient = light.ambient * material.ambient;"
	"		"
	"		vec3 normal = normalize(fragNormal);"
	"		vec3 lightDirection = normalize(light.position - fragPosition);"
	"		float diff = max(dot(normal, lightDirection), 0.0);"
	"		vec4 diffuse = light.diffuse * (diff * material.diffuse);"
	"		"
	"		float shininess = 32;"
	"		vec3 viewDirection = normalize(viewPosition - fragPosition);"
	"		vec3 reflectDirection = reflect(-lightDirection, normal);"
	"		float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), shininess);"
	"		vec4 specular = light.specular * (spec * material.specular);"
	"		"
	"		vec4 lightMaterialColor = ambient + diffuse + specular + material.emissive;"
	"		"
	"		if(!texture)"
	"		{"
	"			gl_FragColor = lightMaterialColor;"
	"		}"
	"		else"
	"		{"
	"			gl_FragColor = texture2D(image, fragTexture) * lightMaterialColor;"
	"		}"
	"	#REPLACE_CLOSE#"
	"}");

	static std::basic_string<GLchar> getFragmentShader(FragmentShader::TYPE type)
	{
		std::basic_string<GLchar> fragmentCode = FragmentShader::codeFragment;

		switch(type)
		{
			case FragmentShader::TYPE::SHADERF:
				{
					ShaderProgram::replaceShaderCode(fragmentCode, std::basic_string<GLchar>("#REPLACE_LIGHT_IF#"), std::basic_string<GLchar>("if(!lighting){ gl_FragColor = color; } else {"));
					ShaderProgram::replaceShaderCode(fragmentCode, std::basic_string<GLchar>("#REPLACE_CLOSE#"), std::basic_string<GLchar>("}"));
				}
				break;

			case FragmentShader::TYPE::FIXEDF:
				{
					ShaderProgram::replaceShaderCode(fragmentCode, std::basic_string<GLchar>("#REPLACE_LIGHT_IF#"), std::basic_string<GLchar>(""));
					ShaderProgram::replaceShaderCode(fragmentCode, std::basic_string<GLchar>("#REPLACE_CLOSE#"), std::basic_string<GLchar>(""));
				}
				break;
		}

		//ShaderProgram::replaceShaderCode(fragmentCode, std::basic_string<GLchar>(";"), std::basic_string<GLchar>(";\n"));

		return fragmentCode;
	}

	const std::basic_string<GLchar> codeFontFragment(
	"#version 120\n"

	"uniform vec3 color = vec3(0.0, 0.0, 0.0);"

	"uniform sampler2D text;"

	"varying vec2 textPosition;"

	"void main()"
	"{"
	"	vec4 sampled = vec4(1.0, 1.0, 1.0, texture2D(text, textPosition).r);"
	"	"
	"	gl_FragColor = vec4(color, 1.0) * sampled;"
	"}");
}