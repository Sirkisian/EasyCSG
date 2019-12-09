#include "light.hpp"

Light::Light(_IN_(GLushort & lightSource)):
lightSource(lightSource)
{
	std::array<GLfloat, 3> defaultPosition{0.0f, 0.0f, 0.0f};
	this->lightPosition = defaultPosition;
}

Light::~Light()
{
}

GLvoid Light::setLightValues(_IN_(ARRAY4REF(GLfloat, lightValues)), Light::LIGHTTYPE lightType)
{
	this->setLightingValues<Light::LIGHTTYPE>(lightValues, lightType);
}

const std::array<GLfloat, 4>* Light::getLightValues(Light::LIGHTTYPE lightType)
{
	return this->getLightingValues<Light::LIGHTTYPE>(lightType);
}

std::basic_ostream<TCHAR> & operator<<(_INOUT_(std::basic_ostream<TCHAR> & out), _IN_(Light & light))
{
	FileIO::FormatNumArray4Out<GLfloat> formatedFloat(out);

	out << static_cast<Lighting>(light);

	FileIO::Export::push(out, _T("position"));
	std::for_each(light.lightPosition.begin(), light.lightPosition.end(), formatedFloat);
	FileIO::Export::pop(out);

	return out;
}

GLvoid Light::operator<<(_IN_(rapidxml::xml_node<TCHAR>* parentNode))
{
	Lighting::operator<<(parentNode);

	if(parentNode != nullptr)
	{
		rapidxml::xml_node<TCHAR>* node = parentNode->first_node(_T("position"));

		if(node != nullptr)
		{
			Input::list2NumArray(node->value(), ' ', this->lightPosition);
		}
	}
}