#pragma once
//-----
#include "objectc.hpp"
#include "graphicobject.hpp"
//----
//---
//--
class ComparatorVertex
{
	public:
		ComparatorVertex(double* address):
		address(address)
		{
		}

		bool operator()(_IN_(carve::mesh::MeshSet<3>::vertex_t & vertex))
		{
			return this->address == vertex.v.v;
		}

	private:
		double* address;
};

class GraphicObjectC : public GraphicObject
{
	public:
		static const std::array<GLfloat, 4> lineColor;

		GraphicObjectC(carve::mesh::MeshSet<3>* meshSet,
					   _IN_(carve::interpolate::FaceVertexAttr<TSTRUCT> & fv_tex),
					   _IN_(carve::interpolate::FaceAttr<GLuint> & f_tex_num));
		~GraphicObjectC();

		GLvoid drawObject(_IN_(GLuint & shaderProgram), SceneObject::DRAWMODE mode, GLubyte fixedPipeline = 0);

		GraphicObject* clone();

	private:
		GLboolean getVerticesFromMesh();
		GLvoid getIndicesFromMesh();

		GLvoid filePrintf(std::basic_ostream<TCHAR> & out) const;

		GLvoid render();

		std::map<GLushort, std::vector<GLuint>> faces;

		carve::mesh::MeshSet<3>* meshSet;
		carve::interpolate::FaceVertexAttr<TSTRUCT> fv_tex;
		carve::interpolate::FaceAttr<GLuint> f_tex_num;
};