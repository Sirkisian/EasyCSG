#pragma once
//-----
#include "enumconverter.hpp"
#include "lighting.hpp"
//----
//---
//--
class Material : public Lighting
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

		friend std::basic_ostream<TCHAR> & operator<<(_INOUT_(std::basic_ostream<TCHAR> & out), _IN_(Material & material));
		GLvoid operator<<(_IN_(rapidxml::xml_node<TCHAR>* parentNode));

	private:
		std::array<GLfloat, 4> emissive;
};