#include "graphicobjectc.hpp"

const std::array<GLfloat, 4> GraphicObjectC::lineColor{0.9f, 0.1f, 0.1f, 1.0f};

GraphicObjectC::GraphicObjectC(carve::mesh::MeshSet<3>* meshSet,
							   _IN_(carve::interpolate::FaceVertexAttr<TSTRUCT> & fv_tex),
							   _IN_(carve::interpolate::FaceAttr<unsigned int> & f_tex_num)):
GraphicObject(),
meshSet(meshSet),
fv_tex(fv_tex),
f_tex_num(f_tex_num)
{
}

GraphicObjectC::~GraphicObjectC()
{
	if(this->meshSet != nullptr)
	{
		delete this->meshSet;
		this->meshSet = nullptr;
	}
}

void GraphicObjectC::drawObject(_IN_(unsigned int & shaderProgram), SceneObject::DRAWMODE mode, unsigned char fixedPipeline)
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

void GraphicObjectC::render()
{
	for(carve::mesh::MeshSet<3>::face_iter i = this->meshSet->faceBegin(), j = this->meshSet->faceEnd(); i != j; i++)
	{
		carve::mesh::MeshSet<3>::face_t* face = *i;

		glBegin(GL_POLYGON);

		for(carve::mesh::MeshSet<3>::face_t::edge_iter_t k = face->begin(), l = face->end(); k != l; k++)
		{
			glVertex3dv(k->vert->v.v);
		}

		glEnd();
	}
}