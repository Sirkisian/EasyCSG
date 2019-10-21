#pragma once
//-----
#include "graphicobjectc.hpp"
#include "csgworld.hpp"
//----
//---
//--
class Carve
{
	public:
		static GraphicObjectC* calculate(_IN_(std::vector<CsgNode>* nodes));

	private:
		static carve::mesh::MeshSet<3>* createMesh(_IN_(GraphicObjectG * object),
												   _OUT_(carve::interpolate::FaceVertexAttr<TSTRUCT> & fv_tex),
												   _OUT_(carve::interpolate::FaceAttr<unsigned int> & f_tex_num));
};