#pragma once
//-----
#include <gl/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#include "fileio.hpp"
//----
//---
//--
class Camera
{
	public:
		Camera();
		~Camera();

		GLvoid setDefaultValues();

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

		friend std::basic_ostream<TCHAR> & operator<<(_INOUT_(std::basic_ostream<TCHAR> & out), _IN_(Camera & camera));
		GLvoid operator<<(_IN_(rapidxml::xml_node<TCHAR>* node));

	private:
		GLvoid calculateEye();

		glm::mat4 view;

		glm::vec3 up;
		glm::vec3 eye;
		glm::vec3 center;

		GLfloat distance;
		GLfloat angleVertical;
		GLfloat angleHorizontal;
};