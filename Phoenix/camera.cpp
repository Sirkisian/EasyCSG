#include "camera.hpp"

Camera::Camera()
{
	this->setDefaultValues();
}

Camera::~Camera()
{
}

GLvoid Camera::setDefaultValues()
{
	this->distance = 8.0f,
	this->angleVertical = 0.0f,
	this->angleHorizontal = 0.0f;

	this->view = glm::mat4(1.0f);

	this->up = glm::vec3(0.0f, 1.0f, 0.0f);
	this->center = glm::vec3(0.0f, 0.0f, 0.0f);

	this->calculateEye();
}

GLvoid Camera::multViewMatrix(_INOUT_(glm::mat4 & matrix))
{
	this->view = glm::lookAt(this->eye, this->center, this->up);

	matrix *= this->view;
}

GLvoid Camera::setOrbitCamera(GLbyte signHorizontal, GLbyte signVertical, GLbyte signDistance)
{
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

	this->calculateEye();
}

std::basic_ostream<TCHAR> & operator<<(_INOUT_(std::basic_ostream<TCHAR> & out), _IN_(Camera & camera))
{
	FileIO::FormatNumArray4Out<GLfloat> formatedFloat(out);

	std::array<GLfloat, 3> values{camera.angleHorizontal, camera.angleVertical, camera.distance};
	std::for_each(values.begin(), values.end(), formatedFloat);

	return out;
}

GLvoid Camera::operator<<(_IN_(rapidxml::xml_node<TCHAR>* node))
{
	if(node != nullptr)
	{
		std::array<GLfloat, 3> values;
		Input::list2NumArray(node->value(), ' ', values);

		this->angleHorizontal = values[0];
		this->angleVertical = values[1];
		this->distance = values[2];

		this->calculateEye();
	}
}

GLvoid Camera::calculateEye()
{
	GLfloat tmp1, tmp2, tmp3;
	GLfloat halfPI = static_cast<GLfloat>(M_PI) / 180.0f;

	tmp1 = this->angleHorizontal * halfPI;
	tmp2 = this->angleVertical * halfPI; 
	tmp3 = cosf(tmp2);
	this->eye[mCOORDINATE::X] = this->center[mCOORDINATE::X] + (this->distance * sinf(tmp1) * tmp3);
	this->eye[mCOORDINATE::Y] = this->center[mCOORDINATE::Y] + (-this->distance * sinf(tmp2));
	this->eye[mCOORDINATE::Z] = this->center[mCOORDINATE::Z] + (this->distance * cosf(tmp1) * tmp3);
}