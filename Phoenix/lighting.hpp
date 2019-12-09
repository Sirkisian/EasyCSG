#pragma once
//-----
#include <gl/glew.h>
#include "fileio.hpp"
//----
//---
//--
class Lighting
{
	public:
		static const std::vector<std::array<GLfloat, 2>> minMax;

		Lighting();
		~Lighting();

		friend std::basic_ostream<TCHAR> & operator<<(_INOUT_(std::basic_ostream<TCHAR> & out), _IN_(Lighting & lighting));
		GLvoid operator<<(_IN_(rapidxml::xml_node<TCHAR>* parentNode));

	protected:
		template <class T>
		GLvoid setLightingValues(_IN_(ARRAY4REF(GLfloat, lightingValues)), T lightingType)
		{
			switch(lightingType)
			{
				case T::AMBIENT:
					{
						this->ambient = lightingValues;
					}
					break;

				case T::DIFFUSE:
					{
						this->diffuse = lightingValues;
					}
					break;

				case T::SPECULAR:
					{
						this->specular = lightingValues;
					}
					break;
			}
		}

		template <class T>
		const std::array<GLfloat, 4>* getLightingValues(T lightingType)
		{
			switch(lightingType)
			{
				case T::AMBIENT:
					{
						return &this->ambient;
					}
					break;

				case T::DIFFUSE:
					{
						return &this->diffuse;
					}
					break;

				case T::SPECULAR:
					{
						return &this->specular;
					}
					break;
			}

			return nullptr;
		}

		GLboolean setLightingArray(_IN_(std::basic_string<TCHAR> & list), _OUT_(ARRAY4REF(GLfloat, lightingArray)));

		std::array<GLfloat, 4> ambient;
		std::array<GLfloat, 4> diffuse;
		std::array<GLfloat, 4> specular;
};