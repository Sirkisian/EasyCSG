#pragma once
//-----
#include <gl/glew.h>
#include "enumconverter.hpp"
#include "fileio.hpp"
//----
//---
//--
class Light
{
	public:
		enum class LIGHTTYPE : GLubyte {NOLIGHTTYPE, AMBIENT, DIFFUSE, SPECULAR};

		static inline Light::LIGHTTYPE convertLightType(GLubyte lightType)
		{
			return EnumConverter::convert<Light::LIGHTTYPE>(lightType, Light::LIGHTTYPE::AMBIENT, Light::LIGHTTYPE::SPECULAR);
		}

		Light(_IN_(GLushort & lightSource));
		~Light();

		inline GLvoid setLightPosition(_IN_(ARRAY3REF(GLfloat, lightPosition)))
		{
			this->lightPosition = lightPosition;
		}

		inline const std::array<GLfloat, 3>* getLightPosition()
		{
			return &this->lightPosition;
		}

		GLvoid setLightValues(_IN_(ARRAY4REF(GLfloat, lightValues)), Light::LIGHTTYPE lightType);
		const std::array<GLfloat, 4>* getLightValues(Light::LIGHTTYPE lightType);

		inline GLushort getLightSource()
		{
			return lightSource;
		}

		friend std::basic_ostream<TCHAR> & operator<<(std::basic_ostream<TCHAR> & out, _IN_(Light & light));

	private:
		GLushort lightSource;

		std::array<GLfloat, 3> lightPosition;

		std::array<GLfloat, 4> ambient;
		std::array<GLfloat, 4> diffuse;
		std::array<GLfloat, 4> specular;
};