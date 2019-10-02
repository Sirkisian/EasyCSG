#pragma once
//-----
#include "objectc.hpp"
#include "graphicobject.hpp"
//----
//---
//--
class GraphicObjectC : public GraphicObject
{
	public:
		static const std::array<GLfloat, 4> lineColor;

		GraphicObjectC(carve::mesh::MeshSet<3>* meshSet,
					   _IN_(carve::interpolate::FaceVertexAttr<TSTRUCT> & fv_tex),
					   _IN_(carve::interpolate::FaceAttr<unsigned int> & f_tex_num));
		~GraphicObjectC();

		void drawObject(_IN_(unsigned int & shaderProgram), SceneObject::DRAWMODE mode, unsigned char fixedPipeline = 0);

		GraphicObject* clone();

	private:
		void render();

		carve::mesh::MeshSet<3>* meshSet;
		carve::interpolate::FaceVertexAttr<TSTRUCT> fv_tex;
		carve::interpolate::FaceAttr<unsigned int> f_tex_num;
};