#include "light.hpp"

Light::Light(_IN_(GLushort & lightSource)):
lightSource(lightSource)
{
	std::array<GLfloat, 4> noLight{0.0f, 0.0f, 0.0f, 1.0f};

	this->ambient = noLight;
	this->diffuse = noLight;
	this->specular = noLight;

	std::array<GLfloat, 3> defaultPosition{0.0f, 0.0f, 0.0f};
	this->lightPosition = defaultPosition;
}

Light::~Light()
{
}

GLvoid Light::setLightValues(_IN_(ARRAY4REF(GLfloat, lightValues)), Light::LIGHTTYPE lightType)
{
	switch(lightType)
	{
		case Light::LIGHTTYPE::AMBIENT:
			{
				this->ambient = lightValues;
			}
			break;

		case Light::LIGHTTYPE::DIFFUSE:
			{
				this->diffuse = lightValues;
			}
			break;

		case Light::LIGHTTYPE::SPECULAR:
			{
				this->specular = lightValues;
			}
			break;
	}
}

const std::array<GLfloat, 4>* Light::getLightValues(Light::LIGHTTYPE lightType)
{
	switch(lightType)
	{
		case Light::LIGHTTYPE::AMBIENT:
			{
				return &this->ambient;
			}
			break;

		case Light::LIGHTTYPE::DIFFUSE:
			{
				return &this->diffuse;
			}
			break;

		case Light::LIGHTTYPE::SPECULAR:
			{
				return &this->specular;
			}
			break;
	}

	return nullptr;
}