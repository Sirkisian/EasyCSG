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

std::basic_ostream<TCHAR> & operator<<(_INOUT_(std::basic_ostream<TCHAR> & out), _IN_(GraphicLight & graphicLight))
{
	FileIO::Export::push(out, _T("values"));
	out << graphicLight.light;
	FileIO::Export::pop(out);

	if(graphicLight.lightCube != nullptr)
	{
		FileIO::Export::push(out, _T("lightcube"));
		FileIO::Export::push(out, _T("transformation"));
		out << graphicLight.lightCube->transformation;
		FileIO::Export::pop(out);
		FileIO::Export::pop(out);
	}

	return out;
}

GLvoid GraphicLight::operator<<(_IN_(rapidxml::xml_node<TCHAR>* parentNode))
{
	if(parentNode != nullptr)
	{
		rapidxml::xml_node<TCHAR>* node = parentNode->first_node();

		while(node != nullptr)
		{
			if(FileIO::Import::isTag(_T("values"), node))
			{
				this->light << node;
			}
			else if(FileIO::Import::isTag(_T("lightcube"), node))
			{
				if(this->lightCube != nullptr)
				{
					rapidxml::xml_node<TCHAR>* childNode = node->first_node();

					if(FileIO::Import::isTag(_T("transformation"), childNode))
					{
						this->lightCube->transformation << childNode;
					}
				}
			}

			node = node->next_sibling();
		}
	}
}