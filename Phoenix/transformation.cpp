#include "transformation.hpp"

Transformation::Transformation()
{
	this->translation = glm::vec3(0.0f, 0.0f, 0.0f);
	this->scaling = glm::vec3(1.0f, 1.0f, 1.0f);
	this->rotation = glm::vec3(0.0f, 0.0f, 0.0f);
}

Transformation::~Transformation()
{
}

GLvoid Transformation::multModelMatrix(_INOUT_(glm::mat4 & matrix)) const
{
	glm::mat4 model = glm::translate(glm::mat4(1.0f), this->translation);
	model = glm::rotate(model, glm::radians(this->rotation[mCOORDINATE::X]), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(this->rotation[mCOORDINATE::Y]), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(this->rotation[mCOORDINATE::Z]), glm::vec3(0, 0, 1));
	model = glm::scale(model, this->scaling);

	matrix *= model;
}

std::basic_ostream<TCHAR> & operator<<(_INOUT_(std::basic_ostream<TCHAR> & out), _IN_(Transformation & transformation))
{
	FileIO::FormatNumArray4Out<GLfloat> formatedFloat(out);

	FileIO::Export::push(out, _T("translation"));
	std::array<GLfloat, 3> translation{transformation.translation.x, transformation.translation.y, transformation.translation.z};
	std::for_each(translation.begin(), translation.end(), formatedFloat);
	FileIO::Export::pop(out);

	FileIO::Export::push(out, _T("scaling"));
	std::array<GLfloat, 3> scaling{transformation.scaling.x, transformation.scaling.y, transformation.scaling.z};
	std::for_each(scaling.begin(), scaling.end(), formatedFloat);
	FileIO::Export::pop(out);

	FileIO::Export::push(out, _T("rotation"));
	std::array<GLfloat, 3> rotation{transformation.rotation.x, transformation.rotation.y, transformation.rotation.z};
	std::for_each(rotation.begin(), rotation.end(), formatedFloat);
	FileIO::Export::pop(out);

	return out;
}

GLvoid Transformation::operator<<(_IN_(rapidxml::xml_node<TCHAR>* parentNode))
{
	if(parentNode != nullptr)
	{
		std::array<GLfloat, 3> values;

		rapidxml::xml_node<TCHAR>* node = parentNode->first_node();

		while(node != nullptr)
		{
			if(FileIO::Import::isTag(_T("translation"), node))
			{
				Input::list2NumArray(node->value(), ' ', values);
				this->translation = glm::vec3(values[0], values[1], values[2]);
			}
			else if(FileIO::Import::isTag(_T("scaling"), node))
			{
				Input::list2NumArray(node->value(), ' ', values);
				this->scaling = glm::vec3(values[0], values[1], values[2]);
			}
			else if(FileIO::Import::isTag(_T("rotation"), node))
			{
				Input::list2NumArray(node->value(), ' ', values);
				this->rotation  = glm::vec3(values[0], values[1], values[2]);
			}

			node = node->next_sibling();
		}
	}
}

GLvoid Transformation::transform(_INOUT_(glm::vec3 & transformation), _IN_(GLfloat & x), _IN_(GLfloat & y), _IN_(GLfloat & z), _IN_(Transformation::ACTION & action))
{
	switch(action)
	{
		case Transformation::ACTION::ADD:
		{
			transformation[mCOORDINATE::X] += x;
			transformation[mCOORDINATE::Y] += y;
			transformation[mCOORDINATE::Z] += z;
		}
		break;

		case  Transformation::ACTION::SET:
		{
			transformation[mCOORDINATE::X] = x;
			transformation[mCOORDINATE::Y] = y;
			transformation[mCOORDINATE::Z] = z;
		}
		break;
	}
}