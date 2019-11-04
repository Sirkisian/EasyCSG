#include "graphicobjectc.hpp"

const std::array<GLfloat, 4> GraphicObjectC::lineColor{0.9f, 0.1f, 0.1f, 1.0f};

GraphicObjectC::GraphicObjectC(carve::mesh::MeshSet<3>* meshSet,
							   _IN_(carve::interpolate::FaceVertexAttr<TSTRUCT> & fv_tex),
							   _IN_(carve::interpolate::FaceAttr<GLuint> & f_tex_num)):
GraphicObject(),
meshSet(meshSet),
fv_tex(fv_tex),
f_tex_num(f_tex_num)
{
	if(this->getVerticesFromMesh())
	{
		this->getIndicesFromMesh();
	}
}

GraphicObjectC::~GraphicObjectC()
{
	if(this->meshSet != nullptr)
	{
		delete this->meshSet;
		this->meshSet = nullptr;
	}
}

void GraphicObjectC::drawObject(_IN_(GLuint & shaderProgram), SceneObject::DRAWMODE mode, GLubyte fixedPipeline)
{
	this->setShaderModelVar(shaderProgram);

	switch(mode)
	{
		case SceneObject::DRAWMODE::WIRE:
			{
				ShaderProgram::setVector<4>(shaderProgram, std::basic_string<GLchar>("color"), &GraphicObjectC::lineColor);

				glLineWidth(1.5f);
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				this->render();
			}
			break;

		case SceneObject::DRAWMODE::COLOR:
			{
				ShaderProgram::setVector<4>(shaderProgram, std::basic_string<GLchar>("color"), &this->objectColor);

				glPolygonOffset(1.0f, 1.0f);

				glEnable(GL_POLYGON_OFFSET_FILL);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				this->render();
				glDisable(GL_POLYGON_OFFSET_FILL);

				ShaderProgram::setVector<4>(shaderProgram, std::basic_string<GLchar>("color"), &GraphicObjectC::lineColor);

				glLineWidth(1.5f);
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				this->render();
			}
			break;

		case SceneObject::DRAWMODE::SELECT:
			{
				glPolygonMode(GL_FRONT, GL_FILL);
				this->render();
			}
			break;
	}
}

GraphicObject* GraphicObjectC::clone()
{
	GraphicObjectC* clone = nullptr;

	carve::mesh::MeshSet<3>* meshSetClone = this->meshSet->clone();

	if(meshSetClone != nullptr)
	{
		clone = new GraphicObjectC(meshSetClone, this->fv_tex, this->f_tex_num);

		if(clone != nullptr)
		{
			clone->transformation = this->transformation;
			clone->material = this->material;
		}
	}

	return clone;
}

GLboolean GraphicObjectC::getVerticesFromMesh()
{
	size_t size = this->meshSet->vertex_storage.size();

	std::vector<GVERTEX>* vertices = new std::vector<GVERTEX>(size);

	if(vertices != nullptr)
	{
		for(size_t i = 0; i < size; i++)
		{
			(*vertices)[i].vertex[mCOORDINATE::X] = static_cast<GLfloat>(this->meshSet->vertex_storage[i].v.x);
			(*vertices)[i].vertex[mCOORDINATE::Y] = static_cast<GLfloat>(this->meshSet->vertex_storage[i].v.y);
			(*vertices)[i].vertex[mCOORDINATE::Z] = static_cast<GLfloat>(this->meshSet->vertex_storage[i].v.z);
		}

		this->vertices = vertices;

		return GL_TRUE;
	}

	return GL_FALSE;
}

GLvoid GraphicObjectC::getIndicesFromMesh()
{
	size_t position;
	carve::mesh::MeshSet<3>::face_t* face = nullptr;
	std::vector<carve::mesh::MeshSet<3>::vertex_t>::const_iterator location;
	std::vector<carve::mesh::MeshSet<3>::vertex_t>::const_iterator storageBegin = this->meshSet->vertex_storage.begin();
	std::vector<carve::mesh::MeshSet<3>::vertex_t>::const_iterator storageEnd = this->meshSet->vertex_storage.end();

	for(carve::mesh::MeshSet<3>::face_iter i = this->meshSet->faceBegin(), j = this->meshSet->faceEnd(); i != j; i++)
	{
		face = *i;

		std::vector<GLuint> objectFace;
		std::pair<std::map<GLuint, std::vector<GLuint>>::iterator, bool> inserted = this->faces.insert({face->n_edges, objectFace});

		for(carve::mesh::MeshSet<3>::face_t::edge_iter_t k = face->begin(), l = face->end(); k != l; k++)
		{
			location = std::find_if(storageBegin, storageEnd, ComparatorVertex(k->vert->v.v));

			if(location != storageEnd)
			{
				position = std::distance(storageBegin, location);

				ExceptionHandler::push_back<GLuint>(inserted.first->second, position);
			}
		}
	}
}

GLvoid GraphicObjectC::filePrintf(std::basic_ostream<TCHAR> & out) const
{
	FileIO::FormatNumber4Out<GLdouble> formatedDouble(out);
	FileIO::FormatNumber4Out<GLint> formatedInt(out);

	FileIO::Export::push(out, _T("type"));
	out << _T("C") << FileIO::Export::pop;

	GraphicObject::filePrintf(out);

	FileIO::Export::push(out, _T("vertices"));
	for(std::vector<carve::mesh::MeshSet<3>::vertex_t>::const_iterator i = this->meshSet->vertex_storage.begin(), j = this->meshSet->vertex_storage.end(); i != j; i++)
	{
		FileIO::Export::push(out, _T("v"));
		std::for_each(i->v.v, i->v.v + 3, formatedDouble);
		FileIO::Export::pop(out);
	}
	FileIO::Export::pop(out);

	size_t size, last;
	FileIO::Export::push(out, _T("faces"));
	for(std::map<GLuint, std::vector<GLuint>>::const_iterator i = this->faces.begin(), j = this->faces.end(); i != j; i++)
	{
		size = (i->second.size() / i->first) - 1;

		for(size_t k = 0; k <= size; k++)
		{
			FileIO::Export::push(out, _T("f"));

			last = i->first * (k + 1);

			if(k == size)
			{
				last--;
			}

			std::for_each(&(i->second[i->first * k]), &(i->second[last]), formatedInt);
			FileIO::Export::pop(out);
		}
	}
	FileIO::Export::pop(out);

	//+ textures
}

void GraphicObjectC::render()
{
	carve::mesh::MeshSet<3>::face_t* face = nullptr;

	for(carve::mesh::MeshSet<3>::face_iter i = this->meshSet->faceBegin(), j = this->meshSet->faceEnd(); i != j; i++)
	{
		face = *i;

		glBegin(GL_POLYGON);

		for(carve::mesh::MeshSet<3>::face_t::edge_iter_t k = face->begin(), l = face->end(); k != l; k++)
		{
			glVertex3dv(k->vert->v.v);
		}

		glEnd();
	}
}