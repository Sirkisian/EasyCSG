#include "object.hpp"

const GLfloat Object2D::floatPi = static_cast<GLfloat>(M_PI);

//	Axis
//
//    o
//    |
//    |
//    o------o
//   /
//  /
// o
//

std::vector<GVERTEX>* Object2D::axis(_IN_(GLfloat & length), _OUT_(GLenum & primitiveType))
{
	std::vector<GVERTEX>* vertices = new std::vector<GVERTEX>(axisVertexCount);

	if(vertices == nullptr)
	{
		_ZERO_OUT_ARGS(primitiveType);

		return nullptr;
	}

	(*vertices)[1].vertex[mCOORDINATE::X] = length;
	(*vertices)[3].vertex[mCOORDINATE::Y] = length;
	(*vertices)[5].vertex[mCOORDINATE::X] = -((sinf(Object2D::toRadians(45)) * length) / sinf(Object2D::toRadians(90)));
	(*vertices)[5].vertex[mCOORDINATE::Y] = (*vertices)[5].vertex[mCOORDINATE::X];

	primitiveType = GL_LINES;

	return vertices;
}

std::vector<GVERTEX>* Object2D::triangleAxis(_IN_(GLfloat & size), _IN_(GLfloat & offset), _OUT_(GLenum & primitiveType))
{
	std::vector<GVERTEX>* vertices = new std::vector<GVERTEX>(triangleAxisVertexCount);

	if(vertices == nullptr)
	{
		_ZERO_OUT_ARGS(primitiveType);

		return nullptr;
	}

	GLfloat tmp1, tmp2, tmp3, tmp4;

	GLfloat s = size;
	GLfloat hS = size / 2.0f;
	GLfloat o = offset;

	tmp1 = -((sinf(Object2D::toRadians(45)) * o) / sinf(Object2D::toRadians(90))); //x = y for offset
	tmp2 = -((sinf(Object2D::toRadians(45)) * s) / sinf(Object2D::toRadians(90)));

	tmp3 = ((sinf(Object2D::toRadians(45)) * hS) / sinf(Object2D::toRadians(75))); //x
	tmp4 = ((sinf(Object2D::toRadians(45)) * hS) / sinf(Object2D::toRadians(90))); //y

	std::vector<GVERTEX> tempVertices
	{{    o,   -hS, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f},
	 {s + o,  0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f},
	 {    o,    hS, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f},

	 {  -hS,     o, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f},
	 {   hS,     o, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f},
	 { 0.0f, s + o, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f},

	 {tmp1 - tmp3, tmp1 + tmp4, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f},
	 {tmp1 + tmp2, tmp1 + tmp2, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f},
	 {tmp1 + tmp3, tmp1 - tmp4, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f}};

	std::copy(tempVertices.begin(), tempVertices.end(), vertices->begin());

	primitiveType = GL_TRIANGLES;

	return vertices;
}

std::vector<GVERTEX>* Object2D::basicCube(_IN_(GLfloat & size), GLfloat offset, _OUT_(GLenum & primitiveType), _OUT_(std::vector<GLuint>** indices))
{
	std::vector<GVERTEX>* vertices = Object2D::alocHelper(Object2D::basicCubeVertexCount, 24, indices);

	if(vertices == nullptr)
	{
		_ZERO_OUT_ARGS(primitiveType, *indices);

		return nullptr;
	}

	GLfloat x = size / 2.0f;
	GLfloat y = x;
	GLfloat o = offset;

	std::vector<GVERTEX> tempVertices
	{{    -x,     -y,  1.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f},
	 {     x,     -y,  1.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f},
	 {     x,      y,  1.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f},
	 {    -x,      y,  1.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f},

	 { x + o, -y + o, -1.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f},
	 {-x + o, -y + o, -1.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f},
	 {-x + o,  y + o, -1.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f},
	 { x + o,  y + o, -1.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f}};

	std::copy(tempVertices.begin(), tempVertices.end(), vertices->begin());

	std::vector<GLuint> tempIndices
	{0, 1, 2, 3,
	 4, 5, 6, 7,
	 1, 4, 7, 2,
	 5, 0, 3, 6,
	 3, 2, 7, 6,
	 0, 5, 4, 1};

	std::copy(tempIndices.begin(), tempIndices.end(), (*indices)->begin());

	primitiveType = GL_QUADS;

	return vertices;
}

std::vector<GVERTEX>* Object2D::alocHelper(GLuint vertexCount, GLuint indicesCount, _OUT_(std::vector<GLuint>** indices))
{
	std::vector<GVERTEX>* vertices = new std::vector<GVERTEX>(vertexCount);

	if(vertices == nullptr)
	{
		return nullptr;
	}

	*indices = new std::vector<GLuint>(indicesCount);

	if(*indices == nullptr)
	{
		delete vertices;

		return nullptr;
	}

	std::fill((*indices)->begin(), (*indices)->end(), 0);

	return vertices;
}

const std::array<std::basic_string<TCHAR>, 5> Object3D::primitiveNames{_T("cube"), _T("sphere"), _T("cylinder"), _T("cone"), _T("pyramid")};

std::vector<GVERTEX>* Object3D::axis(_IN_(GLfloat & length), _OUT_(GLenum & primitiveType))
{
	std::vector<GVERTEX>* vertices = Object2D::axis(length, primitiveType);

	if(vertices != nullptr)
	{
		(*vertices)[5].vertex[mCOORDINATE::X] = 0.0f;
		(*vertices)[5].vertex[mCOORDINATE::Y] = 0.0f;
		(*vertices)[5].vertex[mCOORDINATE::Z] = length;
	}

	return vertices;
}

//	Grid
//
// o----o----o----o
// |    |    |    |
// o----|----|----o
// |    |    |    |
// o----|----|----o
// |    |    |    |
// o----o----o----o
//

std::vector<GVERTEX>* Object3D::grid(_IN_(GLubyte & lineCount), _OUT_(GLenum & primitiveType))
{
	std::vector<GVERTEX>* vertices = new std::vector<GVERTEX>(lineCount * 4);

	if(vertices == nullptr)
	{
		_ZERO_OUT_ARGS(primitiveType);

		return nullptr;
	}

	GLushort tmp1, tmp2, tmp3, tmp4;

	GLfloat n = (0.5f * (lineCount - 1)) / 2;

	for(GLushort i = 0; i < lineCount; i++)
	{
		tmp1 = 2 * i;
		(*vertices)[tmp1].vertex[mCOORDINATE::X] = -n;
		(*vertices)[tmp1].vertex[mCOORDINATE::Y] = 0;
		(*vertices)[tmp1].vertex[mCOORDINATE::Z] = -n + (0.5f * i);

		tmp2 = (2 * i) + 1;
		(*vertices)[tmp2].vertex[mCOORDINATE::X] = n;
		(*vertices)[tmp2].vertex[mCOORDINATE::Y] = 0;
		(*vertices)[tmp2].vertex[mCOORDINATE::Z] = (*vertices)[tmp1].vertex[mCOORDINATE::Z];
	}

	GLushort offset = lineCount * 2;

	for(GLushort i = 0; i < lineCount; i++)
	{
		tmp1 = 2 * i;
		tmp3 = offset + tmp1;
		(*vertices)[tmp3].vertex[mCOORDINATE::X] = (*vertices)[tmp1].vertex[mCOORDINATE::Z];
		(*vertices)[tmp3].vertex[mCOORDINATE::Y] = 0;
		(*vertices)[tmp3].vertex[mCOORDINATE::Z] = (*vertices)[tmp1].vertex[mCOORDINATE::X];
		
		tmp2 = (2 * i) + 1;
		tmp4 = offset + tmp2;
		(*vertices)[tmp4].vertex[mCOORDINATE::X] = (*vertices)[tmp2].vertex[mCOORDINATE::Z];
		(*vertices)[tmp4].vertex[mCOORDINATE::Y] = 0;
		(*vertices)[tmp4].vertex[mCOORDINATE::Z] = (*vertices)[tmp2].vertex[mCOORDINATE::X];
	}

	primitiveType = GL_LINES;

	return vertices;
}

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

std::vector<GVERTEX>* Object3D::basicCube(_IN_(GLfloat & size), _OUT_(GLenum & primitiveType), _OUT_(std::vector<GLuint>** indices))
{
	std::vector<GVERTEX>* vertices = Object2D::basicCube(size, 0, primitiveType, indices);

	if(vertices != nullptr)
	{
		size_t vertexCount = vertices->size();

		GLfloat sizeZ = size / 2.0f;
		size_t halfZ = vertexCount / 2;

		for(GLubyte i = 0; i < vertexCount; i++)
		{
			if(i == halfZ)
			{
				sizeZ = -sizeZ;
			}

			(*vertices)[i].vertex[mCOORDINATE::Z] = sizeZ;
		}
	}

	return vertices;
}

//	Cube
//
//   o--------o
//  /|       /|
// o--------o |
// | |      | |
// | o------|-o
// |/       |/
// o--------o
//

const std::array<GLfloat, 3> Object3D::cubeParameters{1.0f, 1.0f, 1.0f};

std::vector<GVERTEX>* Object3D::primitiveCube(GLfloat length, GLfloat height, GLfloat depth, _OUT_(GLenum & primitiveType), _OUT_(std::vector<GLuint>** indices))
{
	std::vector<GVERTEX>* vertices = Object2D::alocHelper(Object3D::primitiveCubeVertexCount, 36, indices);

	if(vertices == nullptr)
	{
		_ZERO_OUT_ARGS(primitiveType, *indices);

		return nullptr;
	}

	GLfloat x = length / 2.0f;
	GLfloat y = height / 2.0f;
	GLfloat z = depth / 2.0f;

	std::vector<GVERTEX> tempVertices
	{{-x, -y,  z,	 0.0f,  0.0f,  1.0f,	0.0f, 0.0f}, //front face
	 { x, -y,  z,	 0.0f,  0.0f,  1.0f,	1.0f, 0.0f},
	 { x,  y,  z,	 0.0f,  0.0f,  1.0f,	1.0f, 1.0f},
	 {-x,  y,  z,	 0.0f,  0.0f,  1.0f,	0.0f, 1.0f},

	 { x, -y, -z,	 0.0f,  0.0f, -1.0f,	0.0f, 0.0f}, //back face
	 {-x, -y, -z,	 0.0f,  0.0f, -1.0f,	1.0f, 0.0f},
	 {-x,  y, -z,	 0.0f,  0.0f, -1.0f,	1.0f, 1.0f},
	 { x,  y, -z,	 0.0f,  0.0f, -1.0f,	0.0f, 1.0f},

	 { x, -y,  z,	 1.0f,  0.0f,  0.0f,	0.0f, 0.0f}, //right face
	 { x, -y, -z,	 1.0f,  0.0f,  0.0f,	1.0f, 0.0f},
	 { x,  y, -z,	 1.0f,  0.0f,  0.0f,	1.0f, 1.0f},
	 { x,  y,  z,	 1.0f,  0.0f,  0.0f,	0.0f, 1.0f},

	 {-x, -y, -z,	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f}, //left face
	 {-x, -y,  z,	-1.0f,  0.0f,  0.0f,	1.0f, 0.0f},
	 {-x,  y,  z,	-1.0f,  0.0f,  0.0f,	1.0f, 1.0f},
	 {-x,  y, -z,	-1.0f,  0.0f,  0.0f,	0.0f, 1.0f},

	 {-x,  y,  z,	 0.0f,  1.0f,  0.0f,	0.0f, 0.0f}, //top face
	 { x,  y,  z,	 0.0f,  1.0f,  0.0f,	1.0f, 0.0f},
	 { x,  y, -z,	 0.0f,  1.0f,  0.0f,	1.0f, 1.0f},
	 {-x,  y, -z,	 0.0f,  1.0f,  0.0f,	0.0f, 1.0f},

	 {-x, -y, -z,	 0.0f, -1.0f,  0.0f,	0.0f, 0.0f}, //bottom face
	 { x, -y, -z,	 0.0f, -1.0f,  0.0f,	1.0f, 0.0f},
	 { x, -y,  z,	 0.0f, -1.0f,  0.0f,	1.0f, 1.0f},
	 {-x, -y,  z,	 0.0f, -1.0f,  0.0f,	0.0f, 1.0f}};

	std::copy(tempVertices.begin(), tempVertices.end(), vertices->begin());

	GLubyte tmp;

	for(GLubyte i = 0, j = 0; i < 6; i++)
	{
		tmp = 4 * i;

		(**indices)[j++] = tmp;
		(**indices)[j++] = tmp + 1;
		(**indices)[j++] = tmp + 2;

		(**indices)[j++] = tmp;
		(**indices)[j++] = tmp + 2;
		(**indices)[j++] = tmp + 3;
	}

	primitiveType = GL_TRIANGLES;

	return vertices;
}

//	Sphere
//
// o20--o21--o22--o23--o24=o20
// |    |    |    |    |
// o15--o16--o17--o18--o19=o15
// |    |    |    |    |
// o10--o11--o12--o13--o14=o10
// |    |    |    |    |
// o5---o6---o7---o8---o9=o5
// |    |    |    |    |
// o0---o1---o2---o3---o4=o0
//
//4*4
//

const std::array<GLfloat, 3> Object3D::sphereParameters = {1.0f, 15.0f, 15.0f};

std::vector<GVERTEX>* Object3D::primitiveSphere(GLfloat radius, GLushort lats, GLushort longs, _OUT_(GLenum & primitiveType), _OUT_(std::vector<GLuint>** indices))
{
	std::vector<GVERTEX>* vertices = Object2D::alocHelper((lats + 1) * (longs + 1), (lats - 1) * longs * 6, indices);

	if(vertices == nullptr)
	{
		_ZERO_OUT_ARGS(primitiveType, *indices);

		return nullptr;
	}

	GLfloat tmp1, tmp2, tmp3;
	GLuint tmp4;

	GLfloat phiStep = Object2D::floatPi / lats, thetaStep = (2.0f * Object2D::floatPi) / longs;
	GLfloat rowStep = 1.0f / lats, colStep = 1.0f / longs;
	GLfloat textureV;

	GLboolean between;

	GLuint i = 0, j = 0;
	for(GLint phi = lats; phi >= 0; phi--)
	{
		tmp1 = phiStep * phi;
		tmp2 = sinf(tmp1);

		between = phi != lats && phi != 0;

		textureV = 1.0f - (rowStep * phi);

		for(GLint theta = longs; theta >= 0; theta--, i++)
		{
			(*vertices)[i].normal[1] = (*vertices)[i].vertex[mCOORDINATE::Y] = cosf(tmp1);
			(*vertices)[i].vertex[mCOORDINATE::Y] *= radius;

			if(between)
			{
				tmp3 = thetaStep * theta;

				(*vertices)[i].normal[0] = (*vertices)[i].vertex[mCOORDINATE::X] = cosf(tmp3) * tmp2;
				(*vertices)[i].vertex[mCOORDINATE::X] *= radius;

				(*vertices)[i].normal[2] = (*vertices)[i].vertex[mCOORDINATE::Z] = sinf(tmp3) * tmp2;
				(*vertices)[i].vertex[mCOORDINATE::Z] *= radius;
			}

			(*vertices)[i].texture[0] = 1.0f - (colStep * theta);
			(*vertices)[i].texture[1] = textureV;

			if(phi > 0)
			{
				if(theta > 0)
				{
					tmp4 = (longs + 1) + i;

					if(phi != lats)
					{
						(**indices)[j++] = i;
						(**indices)[j++] = i + 1;
						(**indices)[j++] = tmp4 + 1;
					}

					if(phi != 1)
					{
						(**indices)[j++] = i;
						(**indices)[j++] = tmp4 + 1;
						(**indices)[j++] = tmp4;
					}
				}
			}
		}
	}

	primitiveType = GL_TRIANGLES;

	return vertices;
}

//	Cylinder
//
//     o25--o26--o27--o28--o29=o25
//     |    |    |    |    |
//     o20--o21--o22--o23--o24=o20
//    //   //   //   //   //
//   o15--o16--o17--o18--o19=o15
//   |    |    |    |    |
//   o10--o11--o12--o13--o14=o10
//  //   //   //   //   //
// o5---o6---o7---o8---o9=o5
// |    |    |    |    |
// o0---o1---o2---o3---o4=o0
//
//4 slices
//

const std::array<GLfloat, 3> Object3D::cylinderParameters = {0.5f, 5.0, 15.0f};

std::vector<GVERTEX>* Object3D::primitiveCylinder(GLfloat radius, GLfloat height, GLushort slices, _OUT_(GLenum & primitiveType), _OUT_(std::vector<GLuint>** indices))
{
	std::vector<GVERTEX>* vertices = Object2D::alocHelper((slices + 1) * 6, 2 * slices * 6, indices);

	if(vertices == nullptr)
	{
		_ZERO_OUT_ARGS(primitiveType, *indices);

		return nullptr;
	}

	GLfloat tmp1;
	GLuint tmp2;

	GLfloat thetaStep = (2.0f * Object2D::floatPi) / slices;
	GLfloat colStep = 1.0f / slices;
	GLfloat lengthInverse = 1.0f / radius;
	GLfloat positionY = -1.0f * (height / 2.0f);

	GLboolean between;

	GLuint i = 0, j = 0;
	for(GLubyte row = 0; row <= 5; row++)
	{
		between = row != 0 && row != 5;

		if(row == 3)
		{
			positionY *= -1.0f;
		}

		for(GLint theta = slices; theta >= 0; theta--, i++)
		{
			(*vertices)[i].vertex[mCOORDINATE::Y] = positionY * radius;

			if(between)
			{
				tmp1 = thetaStep * theta;

				(*vertices)[i].vertex[mCOORDINATE::X] = cosf(tmp1) * radius;

				(*vertices)[i].vertex[mCOORDINATE::Z] = sinf(tmp1) * radius;
			}

			if(row <= 1)
			{
				(*vertices)[i].normal[1] = -1;
			}
			else if(row <= 3)
			{
				(*vertices)[i].normal[0] = (*vertices)[i].vertex[mCOORDINATE::X] * lengthInverse;
				(*vertices)[i].normal[2] = (*vertices)[i].vertex[mCOORDINATE::Z] * lengthInverse;

				(*vertices)[i].texture[0] = 1.0f - (colStep * theta);

				if(row == 3)
				{
					(*vertices)[i].texture[1] = 1;
				}
			}
			else
			{
				(*vertices)[i].normal[1] = 1;
			}

			if(row % 2 == 0)
			{
				if(theta > 0)
				{
					tmp2 = (slices + 1) + i;

					if(row != 0)
					{
						(**indices)[j++] = i;
						(**indices)[j++] = i + 1;
						(**indices)[j++] = tmp2 + 1;
					}

					if(row != 4)
					{
						(**indices)[j++] = i;
						(**indices)[j++] = tmp2 + 1;
						(**indices)[j++] = tmp2;
					}
				}
			}
		}
	}

	primitiveType = GL_TRIANGLES;

	return vertices;
}