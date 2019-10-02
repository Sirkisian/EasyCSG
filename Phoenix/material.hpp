#pragma once
//-----
#include <gl/glew.h>
#include "macros.hpp"
#include "enumconverter.hpp"
//----
//---
//--
class Material
{
	public:
		enum class MATERIALTYPE : GLubyte {NOMATERIALTYPE, AMBIENT, DIFFUSE, SPECULAR, EMISSIVE};

		static inline Material::MATERIALTYPE convertMaterialType(GLubyte materialType)
		{
			return EnumConverter::convert<Material::MATERIALTYPE>(materialType, Material::MATERIALTYPE::AMBIENT, Material::MATERIALTYPE::EMISSIVE);
		}

		Material();
		~Material();

		GLvoid setMaterialValues(_IN_(ARRAY4REF(GLfloat, materialValues)), Material::MATERIALTYPE materialType);
		const std::array<GLfloat, 4>* getMaterialValues(Material::MATERIALTYPE materialType);

	private:
		std::array<GLfloat, 4> ambient;
		std::array<GLfloat, 4> diffuse;
		std::array<GLfloat, 4> specular;
		std::array<GLfloat, 4> emissive;
};