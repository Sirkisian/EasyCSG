#include "graphiclight.hpp"

GraphicLight::GraphicLight(_IN_(GLushort & lightSource)):
light(lightSource)
{
	this->lightCube = ObjectManager::createBasicCube(0.1f, 0.0f, ObjectManager::DIMENSION::Object3D);

	if(this->lightCube != nullptr)
	{
		std::array<GLfloat, 4> material{1.0f, 1.0f, 1.0f, 1.0f};

		this->lightCube->setObjectMaterial(material, Material::MATERIALTYPE::EMISSIVE);
	}
}

GraphicLight::~GraphicLight()
{
	if(this->lightCube != nullptr)
	{
		delete this->lightCube;
		this->lightCube = nullptr;
	}
}

GLvoid GraphicLight::setPosition(_IN_(ARRAY3REF(GLfloat, position)))
{
	this->light.setLightPosition(position);

	if(this->lightCube != nullptr)
	{
		this->lightCube->transformation.translate(position[mCOORDINATE::X], position[mCOORDINATE::Y], position[mCOORDINATE::Z], Transformation::ACTION::SET);
	}
}

std::basic_ostream<TCHAR> & operator<<(std::basic_ostream<TCHAR> & out, _IN_(GraphicLight & graphicLight))
{
	out << graphicLight.light;

	if(graphicLight.lightCube != nullptr)
	{
		FileIO::Export::push(out, _T("lightcube"));
		out << graphicLight.lightCube->transformation << FileIO::Export::pop;
	}

	return out;
}