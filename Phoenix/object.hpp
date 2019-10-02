#pragma once
//-----
#include <vector>
#include <gl/glew.h>
#include <tchar.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "macros.hpp"
//----
//---
typedef struct gvertex
{
	gvertex():
	vertex{0.0f, 0.0f, 0.0f},
	normal{0.0f, 0.0f, 0.0f},
	texture{0.0f, 0.0f}
	{
	}

	gvertex(GLfloat x, GLfloat y, GLfloat z, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat u, GLfloat v):
	vertex{x, y, z},
	normal{nx, ny, nz},
	texture{u, v}
	{
	}

	GLfloat vertex[3];
	GLfloat normal[3];
	GLfloat texture[2];
}GVERTEX;
//--
namespace Object
{
	enum class OBJECTTYPE : GLubyte {NOOBJECTTYPE, CUBE, SPHERE, CYLINDER, CONE, PYRAMID};
}

class Object2D
{
	public:
		static const GLfloat floatPi;

		static const GLubyte axisVertexCount = 6;
		static std::vector<GVERTEX>* axis(_IN_(GLfloat & length), _OUT_(GLenum & primitiveType));

		static const GLubyte triangleAxisVertexCount = 9;
		static std::vector<GVERTEX>* triangleAxis(_IN_(GLfloat & size), _IN_(GLfloat & offset), _OUT_(GLenum & primitiveType));

		static const GLubyte basicCubeVertexCount = 8;
		static std::vector<GVERTEX>* basicCube(_IN_(GLfloat & size), GLfloat offset, _OUT_(GLenum & primitiveType), _OUT_(std::vector<GLuint>** indices));

		static std::vector<GVERTEX>* alocHelper(GLuint vertexCount, GLuint indicesCount, _OUT_(std::vector<GLuint>** indices));

		friend class Object3D;

	private:
		static inline GLfloat toRadians(GLfloat degree)
		{
			return (degree * Object2D::floatPi) / 180.0f; 
		}
};

class Object3D
{
	public:
		static const std::array<std::basic_string<TCHAR>, 5> primitiveNames;

		static std::vector<GVERTEX>* axis(_IN_(GLfloat & length), _OUT_(GLenum & primitiveType));

		static std::vector<GVERTEX>* grid(_IN_(GLubyte & lineCount), _OUT_(GLenum & primitiveType));

		static std::vector<GVERTEX>* basicCube(_IN_(GLfloat & size), _OUT_(GLenum & primitiveType), _OUT_(std::vector<GLuint>** indices));

		static const std::array<GLfloat, 3> cubeParameters;
		static const GLubyte primitiveCubeVertexCount = 24;
		static std::vector<GVERTEX>* primitiveCube(GLfloat length, GLfloat height, GLfloat depth, _OUT_(GLenum & primitiveType), _OUT_(std::vector<GLuint>** indices));

		static const std::array<GLfloat, 3> sphereParameters;
		static std::vector<GVERTEX>* primitiveSphere(GLfloat radius, GLushort lats, GLushort longs, _OUT_(GLenum & primitiveType), _OUT_(std::vector<GLuint>** indices));

		static const std::array<GLfloat, 3> cylinderParameters;
		static std::vector<GVERTEX>* primitiveCylinder(GLfloat radius, GLfloat height, GLushort slices, _OUT_(GLenum & primitiveType), _OUT_(std::vector<GLuint>** indices));
};