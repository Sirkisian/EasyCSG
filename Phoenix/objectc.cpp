#include "objectc.hpp"

const float MeshSet::floatPi = static_cast<float>(M_PI);

//	Cube
//
//   o6-------o7
//  /|       /|
// o3-------o2|
// | |      | |
// | o5-----|-o4
// |/       |/
// o0-------o1
//

carve::mesh::MeshSet<3>* MeshSet::primitiveCube(float length, float height, float depth, unsigned int texture,
												_IN_(carve::math::Matrix & transformation),
												_OUT_(carve::interpolate::FaceVertexAttr<TSTRUCT> & fv_tex),
												_OUT_(carve::interpolate::FaceAttr<unsigned int> & f_tex_num))
{
	carve::input::Options options;
	carve::input::PolyhedronData data;
	carve::mesh::MeshSet<3>* meshSet = nullptr;

	float x = length / 2.0f;
	float y = height / 2.0f;
	float z = depth / 2.0f;

	data.addVertex(transformation * carve::geom::VECTOR(-x, -y,  z));
	data.addVertex(transformation * carve::geom::VECTOR( x, -y,  z));
	data.addVertex(transformation * carve::geom::VECTOR( x,  y,  z));
	data.addVertex(transformation * carve::geom::VECTOR(-x,  y,  z));

	data.addVertex(transformation * carve::geom::VECTOR( x, -y, -z));
	data.addVertex(transformation * carve::geom::VECTOR(-x, -y, -z));
	data.addVertex(transformation * carve::geom::VECTOR(-x,  y, -z));
	data.addVertex(transformation * carve::geom::VECTOR( x,  y, -z));

	data.addFace(0, 1, 2, 3);
	data.addFace(4, 5, 6, 7);
	data.addFace(1, 4, 7, 2);
	data.addFace(5, 0, 3, 6);
	data.addFace(3, 2, 7, 6);
	data.addFace(5, 4, 1, 0);

	meshSet = data.createMesh(options);

	if(meshSet != nullptr)
	{
		for(carve::mesh::MeshSet<3>::face_iter i = meshSet->faceBegin(), j = meshSet->faceEnd(); i != j; i++)
		{
			fv_tex.setAttribute(*i, 0, TSTRUCT(1.0f, 0.0f));
			fv_tex.setAttribute(*i, 1, TSTRUCT(0.0f, 0.0f));
			fv_tex.setAttribute(*i, 2, TSTRUCT(0.0f, 1.0f));
			fv_tex.setAttribute(*i, 3, TSTRUCT(1.0f, 1.0f));
			f_tex_num.setAttribute(*i, texture);
		}
	}

	return meshSet;
}

//	Sphere
//
//           o13
//  /    /   |   \    \
// o9--o10--o11--o12---o9
// |    |    |    |    |
// o5---o6---o7---o8---o5
// |    |    |    |    |
// o1---o2---o3---o4---o1
//  \    \   |   /    /
//           o0
//
//4*4
//

carve::mesh::MeshSet<3>* MeshSet::primitiveSphere(float radius, unsigned short lats, unsigned short longs, unsigned int texture,
												  _IN_(carve::math::Matrix & transformation),
												  _OUT_(carve::interpolate::FaceVertexAttr<TSTRUCT> & fv_tex),
												  _OUT_(carve::interpolate::FaceAttr<unsigned int> & f_tex_num))
{
	carve::input::Options options;
	carve::input::PolyhedronData data;

	float tmp1, tmp2, tmp3;

	float phiStep = MeshSet::floatPi / lats, thetaStep = (2.0f * MeshSet::floatPi) / longs;

	bool between;

	unsigned int lastIndex;
	unsigned int firstOfLastRow;
	MeshSet::getIndexNumbers(lats, longs, lastIndex, firstOfLastRow);

	unsigned int i = 1;
	for(int phi = lats; phi >= 0; phi--)
	{
		tmp1 = phiStep * phi;
		tmp2 = sinf(tmp1);

		between = phi != lats && phi != 0;

		if(!between)
		{
			data.addVertex(transformation * carve::geom::VECTOR(0, cosf(tmp1) * radius, 0));
		}

		if(phi == lats - 1)
		{
			i = 1;
		}

		if(phi > 0)
		{
			for(int theta = longs; theta > 0; theta--)
			{
				if(between)
				{
					tmp3 = thetaStep * theta;

					data.addVertex(transformation * carve::geom::VECTOR(cosf(tmp3) * tmp2 * radius, cosf(tmp1) * radius, sinf(tmp3) * tmp2 * radius));
				}

				MeshSet::setIndices(phi, theta, lats, longs, lastIndex, firstOfLastRow, i, data);
			}
		}
	}

	return data.createMesh(options);
}

//	Cylinder
//
//           o9
//  /    /   |   \    \
// o5---o6---o7---o8---o5
// |    |    |    |    |
// o1---o2---o3---o4---o1
//  \    \   |   /    /
//           o0
//
//4 slices
//

carve::mesh::MeshSet<3>* MeshSet::primitiveCylinder(float radius, float height, unsigned short slices, unsigned int texture,
													_IN_(carve::math::Matrix & transformation),
													_OUT_(carve::interpolate::FaceVertexAttr<TSTRUCT> & fv_tex),
													_OUT_(carve::interpolate::FaceAttr<unsigned int> & f_tex_num))
{
	carve::input::Options options;
	carve::input::PolyhedronData data;

	float tmp;

	float thetaStep = (2.0f * MeshSet::floatPi) / slices;
	float positionY = -1.0f * (height / 2.0f);

	bool between;

	unsigned int lastIndex;
	unsigned int firstOfLastRow;
	MeshSet::getIndexNumbers(3, slices, lastIndex, firstOfLastRow);

	unsigned int i = 1;
	for(char row = 3; row >= 0; row--)
	{
		between = row != 3 && row != 0;

		if(row == 1)
		{
			positionY *= -1.0f;
		}

		if(!between)
		{
			data.addVertex(transformation * carve::geom::VECTOR(0, positionY * radius, 0));
		}

		if(row == 2)
		{
			i = 1;
		}

		if(row > 0)
		{
			for(int theta = slices; theta > 0; theta--)
			{
				if(between)
				{
					tmp = thetaStep * theta;

					data.addVertex(transformation * carve::geom::VECTOR(cosf(tmp) * radius, positionY * radius, sinf(tmp) * radius));
				}

				MeshSet::setIndices(row, theta, 3, slices, lastIndex, firstOfLastRow, i, data);
			}
		}
	}

	return data.createMesh(options);
}

void MeshSet::getIndexNumbers(unsigned short lats, _IN_(unsigned short & longs), _OUT_(unsigned int & lastIndex), _OUT_(unsigned int & firstOfLastRow))
{
	lastIndex = ((lats - 1) * longs) + 1;
	firstOfLastRow = lastIndex - longs;
}

void MeshSet::setIndices(_IN_(int & phi), 
						 _IN_(int & theta),
						 unsigned short lats,
						 _IN_(unsigned short & longs),
						 _IN_(unsigned int & lastIndex),
						 _IN_(unsigned int & firstOfLastRow),
						 _INOUT_(unsigned int & i),
						 _INOUT_(carve::input::PolyhedronData & data))
{
	if(phi == lats)
	{
		if(theta > 1)
		{
			data.addFace(0, i + 1, i);
		}
		else
		{
			data.addFace(0, 1, i);
		}
	}
	else if(phi == 1)
	{
		if(theta > 1)
		{
			data.addFace(i, i + 1, lastIndex);
		}
		else
		{
			data.addFace(i, firstOfLastRow, lastIndex);
		}
	}
	else
	{
		unsigned int tmp = longs + i;

		if(theta > 1)
		{
			data.addFace(i, i + 1, tmp + 1, tmp);
		}
		else
		{
			data.addFace(i, i - (longs - 1), i + 1, tmp);
		}
	}

	i++;
}