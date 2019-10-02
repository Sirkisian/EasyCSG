#pragma once
//-----
#include <gl/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#include "macros.hpp"
//----
//---
//--
class Camera
{
	public:
		Camera();
		~Camera();

		GLvoid multViewMatrix(_INOUT_(glm::mat4 & matrix));

		GLvoid setOrbitCamera(GLbyte signHorizontal, GLbyte signVertical, GLbyte signDistance = 0);

		inline GLvoid setCameraDirection(glm::vec3 direction)
		{
			this->center = direction;
		}

		inline const glm::vec3 & getCameraPosition()
		{
			return this->eye;
		}

	private:
		glm::mat4 view;

		glm::vec3 up;
		glm::vec3 eye;
		glm::vec3 center;

		GLfloat distance;
		GLfloat angleVertical;
		GLfloat angleHorizontal;
};