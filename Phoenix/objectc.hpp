#pragma once
//-----
#include <carve/input.hpp>
#include <carve/interpolator.hpp>
#include "macros.hpp"
#include "exceptionhandler.hpp"
//----
//---
typedef struct tstruct
{
	tstruct():
	u(0.0f),
	v(0.0f)
	{
	}

	tstruct(float u, float v):
	u(u),
	v(v)
	{
	}

	float u;
	float v;
}TSTRUCT;

// interpolated attributes must support scalar multiplication and operator +=
inline TSTRUCT operator*(double s, _IN_(TSTRUCT & t))
{
	return TSTRUCT(t.u * static_cast<float>(s), t.v * static_cast<float>(s));
}

inline TSTRUCT & operator+=(TSTRUCT & t1, _IN_(TSTRUCT & t2))
{
	t1.u += t2.u;
	t1.v += t2.v;

	return t1;
}
//--
class MeshSet
{
	public:
		static const float floatPi;

		static carve::mesh::MeshSet<3>* primitiveCube(float length, float height, float depth, unsigned int texture,
													  _IN_(carve::math::Matrix & transformation),
													  _OUT_(carve::interpolate::FaceVertexAttr<TSTRUCT> & fv_tex),
													  _OUT_(carve::interpolate::FaceAttr<unsigned int> & f_tex_num));

		static carve::mesh::MeshSet<3>* primitiveSphere(float radius, unsigned short lats, unsigned short longs, unsigned int texture,
														_IN_(carve::math::Matrix & transformation),
														_OUT_(carve::interpolate::FaceVertexAttr<TSTRUCT> & fv_tex),
														_OUT_(carve::interpolate::FaceAttr<unsigned int> & f_tex_num));

		static carve::mesh::MeshSet<3>* primitiveCylinder(float radius, float height, unsigned short slices, unsigned int texture,
														  _IN_(carve::math::Matrix & transformation),
														  _OUT_(carve::interpolate::FaceVertexAttr<TSTRUCT> & fv_tex),
														  _OUT_(carve::interpolate::FaceAttr<unsigned int> & f_tex_num));

	private:
		static void getIndexNumbers(unsigned short lats, _IN_(unsigned short & longs), _OUT_(unsigned int & lastIndex), _OUT_(unsigned int & firstOfLastRow));

		static void setIndices(_IN_(int & phi),
							   _IN_(int & theta),
							   unsigned short lats,
							   _IN_(unsigned short & longs),
							   _IN_(unsigned int & lastIndex),
							   _IN_(unsigned int & firstOfLastRow),
							   _INOUT_(unsigned int & i),
							   _INOUT_(carve::input::PolyhedronData & data));
};