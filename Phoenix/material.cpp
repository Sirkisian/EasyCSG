#include "material.hpp"

Material::Material()
{
	std::array<GLfloat, 4> noMaterial{0.0f, 0.0f, 0.0f, 1.0f};
	this->emissive = noMaterial;
}

Material::~Material()
{
}

GLvoid Material::setMaterialValues(_IN_(ARRAY4REF(GLfloat, materialValues)), Material::MATERIALTYPE materialType)
{
	this->setLightingValues<Material::MATERIALTYPE>(materialValues, materialType);

	switch(materialType)
	{
		case Material::MATERIALTYPE::EMISSIVE:
			{
				this->emissive = materialValues;
			}
			break;
	}
}

const std::array<GLfloat, 4>* Material::getMaterialValues(Material::MATERIALTYPE materialType)
{
	const std::array<GLfloat, 4>* material = this->getLightingValues<Material::MATERIALTYPE>(materialType);

	switch(materialType)
	{
		case Material::MATERIALTYPE::EMISSIVE:
			{
				material = &this->emissive;
			}
			break;
	}

	return material;
}

std::basic_ostream<TCHAR> & operator<<(_INOUT_(std::basic_ostream<TCHAR> & out), _IN_(Material & material))
{
	FileIO::FormatNumArray4Out<GLfloat> formatedFloat(out);

	out << static_cast<Lighting>(material);

	FileIO::Export::push(out, _T("emissive"));
	std::for_each(material.emissive.begin(), material.emissive.end(), formatedFloat);
	FileIO::Export::pop(out);

	return out;
}

GLvoid Material::operator<<(_IN_(rapidxml::xml_node<TCHAR>* parentNode))
{
	Lighting::operator<<(parentNode);

	if(parentNode != nullptr)
	{
		rapidxml::xml_node<TCHAR>* node = parentNode->first_node(_T("emissive"));

		if(node != nullptr)
		{
			this->setLightingArray(node->value(), this->emissive);
		}
	}
}