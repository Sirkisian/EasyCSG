#include "graphicobject.hpp"

const std::array<GLfloat, 4> GraphicObject::defaultObjectColor{0.4f, 0.3f, 0.3f, 1.0f};
const std::array<GLfloat, 4> GraphicObject::selectedObjectColor{0.3f, 0.3f, 0.4f, 1.0f};

GraphicObject::GraphicObject():
SceneObject(),
id(0),
ebo(0),
indices(nullptr)
{
	this->objectColor = defaultObjectColor;
}

GraphicObject::GraphicObject(_IN_(std::vector<GVERTEX>* vertices), _IN_(GLenum & primitiveType), _IN_(std::vector<GLuint>* indices)):
SceneObject(vertices, primitiveType),
id(0),
ebo(0),
indices(indices)
{
	this->objectColor = defaultObjectColor;

	glGenBuffers(1, &this->ebo);

	if(this->ebo > 0)
	{
		if(this->bindVao())
		{
			this->bindVbo();
			this->bindEbo();
			this->unbind();
		}
	}
}

GraphicObject::~GraphicObject()
{
	if(this->ebo > 0)
	{
		glDeleteBuffers(1, &this->ebo);
	}

	if(this->indices != nullptr)
	{
		delete this->indices;
		this->indices = nullptr;
	}
}

GLvoid GraphicObject::bindEbo()
{
	if(this->ebo > 0)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * this->indices->size(), this->indices->data(), GL_STATIC_DRAW);
	}
}

std::basic_ostream<TCHAR> & operator<<(std::basic_ostream<TCHAR> & out, _IN_(GraphicObject* object))
{
	object->filePrintf(out);

	return out;
}

GLvoid GraphicObject::filePrintf(std::basic_ostream<TCHAR> & out) const
{
	FileIO::Export::push(out, _T("id"));
	out << this->id << FileIO::Export::pop;

	FileIO::Export::push(out, _T("name"));
	out << this->objectName << FileIO::Export::pop;

	FileIO::Export::push(out, _T("transformation"));
	out << this->transformation << FileIO::Export::pop;
}