#pragma once
//-----
#include <opencsg.h>
#include "graphicobjectg.hpp"
//----
//---
//--
class OpenCsgObject : public OpenCSG::Primitive
{
	public:
		OpenCsgObject(OpenCSG::Operation operation, unsigned int convexity, GraphicObject* graphicObject);
		~OpenCsgObject();

		inline void setShaderProgram(_IN_(unsigned int & shaderProgram))
		{
			this->shaderProgram = shaderProgram;
		}

		void render();

	private:
		unsigned int shaderProgram;

		GraphicObject* graphicObject;
};