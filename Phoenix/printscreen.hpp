#pragma once
//-----
#include <gl/glew.h>
#include "macros.hpp"
//----
//---
//--
class PrintScreen
{
	public:
		static GLvoid post(GLboolean flush);

		PrintScreen();
		~PrintScreen();

		GLvoid createRenderBuffer(std::array<GLushort, 2> size);

		GLvoid saveScreen();
		GLvoid loadScreen();

	private:
		//frame buffer object
		GLuint fbo;
		//render buffer object
		GLuint rbo;

		std::array<GLushort, 2> size;
};