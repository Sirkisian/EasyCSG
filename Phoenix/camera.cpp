#include "camera.hpp"

Camera::Camera():
distance(8.0f),
angleVertical(0.0f),
angleHorizontal(0.0f)
{
	this->view = glm::mat4(1.0f);

	this->up = glm::vec3(0.0f, 1.0f, 0.0f);
	this->center = glm::vec3(0.0f, 0.0f, 0.0f);
	this->eye = glm::vec3(this->angleHorizontal, this->angleVertical, this->distance);
}

Camera::~Camera()
{
}

GLvoid Camera::multViewMatrix(_INOUT_(glm::mat4 & matrix))
{
	this->view = glm::lookAt(this->eye, this->center, this->up);

	matrix *= this->view;
}

GLvoid Camera::setOrbitCamera(GLbyte signHorizontal, GLbyte signVertical, GLbyte signDistance)
{
	GLfloat tmp1, tmp2, tmp3;
	GLfloat halfPI = static_cast<GLfloat>(M_PI) / 180.0f;

	if(signHorizontal != 0)
	{
		this->angleHorizontal += 1.0f * signHorizontal;
	}

	if(signVertical != 0)
	{
		this->angleVertical += 1.0f * signVertical;
	}

	if(signDistance != 0)
	{
		this->distance += 1.0f * signDistance;
	}

	tmp1 = this->angleHorizontal * halfPI;
	tmp2 = this->angleVertical * halfPI; 
	tmp3 = cosf(tmp2);
	this->eye[mCOORDINATE::X] = this->center[mCOORDINATE::X] + (this->distance * sinf(tmp1) * tmp3);
	this->eye[mCOORDINATE::Y] = this->center[mCOORDINATE::Y] + (-this->distance * sinf(tmp2));
	this->eye[mCOORDINATE::Z] = this->center[mCOORDINATE::Z] + (this->distance * cosf(tmp1) * tmp3);
}

std::basic_ostream<TCHAR> & operator<<(std::basic_ostream<TCHAR> & out, _IN_(Camera & camera))
{
	out << std::fixed << camera.angleHorizontal << _T(" ") << camera.angleVertical << _T(" ") << camera.distance;

	return out;
}