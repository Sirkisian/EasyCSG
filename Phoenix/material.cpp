#include "material.hpp"

Material::Material()
{
	std::array<GLfloat, 4> noMaterial{0.0f, 0.0f, 0.0f, 1.0f};

	this->ambient = noMaterial;
	this->diffuse = noMaterial;
	this->specular = noMaterial;
	this->emissive = noMaterial;
}

Material::~Material()
{
}

GLvoid Material::setMaterialValues(_IN_(ARRAY4REF(GLfloat, materialValues)), Material::MATERIALTYPE materialType)
{
	switch(materialType)
	{
		case Material::MATERIALTYPE::AMBIENT:
			{
				this->ambient = materialValues;
			}
			break;

		case Material::MATERIALTYPE::DIFFUSE:
			{
				this->diffuse = materialValues;
			}
			break;

		case Material::MATERIALTYPE::SPECULAR:
			{
				this->specular = materialValues;
			}
			break;

		case Material::MATERIALTYPE::EMISSIVE:
			{
				this->emissive = materialValues;
			}
			break;
	}
}

const std::array<GLfloat, 4>* Material::getMaterialValues(Material::MATERIALTYPE materialType)
{
	switch(materialType)
	{
		case Material::MATERIALTYPE::AMBIENT:
			{
				return &this->ambient;
			}
			break;

		case Material::MATERIALTYPE::DIFFUSE:
			{
				return &this->diffuse;
			}
			break;

		case Material::MATERIALTYPE::SPECULAR:
			{
				return &this->specular;
			}
			break;

		case Material::MATERIALTYPE::EMISSIVE:
			{
				return &this->emissive;
			}
			break;
	}

	return nullptr;
}

std::basic_ostream<TCHAR> & operator<<(std::basic_ostream<TCHAR> & out, _IN_(Material & material))
{
	FileIO::FormatNumber4Out<GLfloat> formatedFloat(out);

	FileIO::Export::push(out, _T("ambient"));
	std::for_each(material.ambient.begin(), material.ambient.end(), formatedFloat);
	FileIO::Export::pop(out);

	FileIO::Export::push(out, _T("diffuse"));
	std::for_each(material.diffuse.begin(), material.diffuse.end(), formatedFloat);
	FileIO::Export::pop(out);

	FileIO::Export::push(out, _T("specular"));
	std::for_each(material.specular.begin(), material.specular.end(), formatedFloat);
	FileIO::Export::pop(out);

	FileIO::Export::push(out, _T("emissive"));
	std::for_each(material.emissive.begin(), material.emissive.end(), formatedFloat);
	FileIO::Export::pop(out);

	return out;
}