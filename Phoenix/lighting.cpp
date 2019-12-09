#include "lighting.hpp"

const std::vector<std::array<GLfloat, 2>> Lighting::minMax{{0.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 1.0f}};

Lighting::Lighting()
{
	std::array<GLfloat, 4> noLighting{0.0f, 0.0f, 0.0f, 1.0f};

	this->ambient = noLighting;
	this->diffuse = noLighting;
	this->specular = noLighting;
}

Lighting::~Lighting()
{
}

std::basic_ostream<TCHAR> & operator<<(_INOUT_(std::basic_ostream<TCHAR> & out), _IN_(Lighting & lighting))
{
	FileIO::FormatNumArray4Out<GLfloat> formatedFloat(out);

	FileIO::Export::push(out, _T("ambient"));
	std::for_each(lighting.ambient.begin(), lighting.ambient.end(), formatedFloat);
	FileIO::Export::pop(out);

	FileIO::Export::push(out, _T("diffuse"));
	std::for_each(lighting.diffuse.begin(), lighting.diffuse.end(), formatedFloat);
	FileIO::Export::pop(out);

	FileIO::Export::push(out, _T("specular"));
	std::for_each(lighting.specular.begin(), lighting.specular.end(), formatedFloat);
	FileIO::Export::pop(out);

	return out;
}

GLvoid Lighting::operator<<(_IN_(rapidxml::xml_node<TCHAR>* parentNode))
{
	if(parentNode != nullptr)
	{
		rapidxml::xml_node<TCHAR>* node = parentNode->first_node();

		while(node != nullptr)
		{
			if(FileIO::Import::isTag(_T("ambient"), node))
			{
				this->setLightingArray(node->value(), this->ambient);
			}
			else if(FileIO::Import::isTag(_T("diffuse"), node))
			{
				this->setLightingArray(node->value(), this->diffuse);
			}
			else if(FileIO::Import::isTag(_T("specular"), node))
			{
				this->setLightingArray(node->value(), this->specular);
			}

			node = node->next_sibling();
		}
	}
}

GLboolean Lighting::setLightingArray(_IN_(std::basic_string<TCHAR> & list), _OUT_(ARRAY4REF(GLfloat, lightingArray)))
{
	std::array<GLfloat, 4> values;

	Input::list2NumArray(list, ' ', values);

	if(Input::checkFloatArrayValues(values, Lighting::minMax))
	{
		std::copy(values.begin(), values.end(), lightingArray.begin());

		return true;
	}

	return false;
}