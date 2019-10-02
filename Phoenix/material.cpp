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