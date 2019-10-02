#include "project.hpp"

Project::Project()
{
	this->graphicWorld.addSceneAxis(9.0f);
	this->graphicWorld.addSceneGrid(36);
	this->graphicWorld.addTransformationAxis(50.0f, 20.0f, 30.0f);

	std::array<GLfloat, 4> light{1.0f, 1.0f, 1.0f, 1.0f};
	this->graphicWorld.addGraphicLight(GL_LIGHT0);
	this->graphicWorld.setLightValues(GL_LIGHT0, light, Light::LIGHTTYPE::AMBIENT);
	this->graphicWorld.setLightValues(GL_LIGHT0, light, Light::LIGHTTYPE::DIFFUSE);
	this->graphicWorld.setLightValues(GL_LIGHT0, light, Light::LIGHTTYPE::SPECULAR);

	this->csgWorld.addCsgTree(_T("default"));
}

Project::~Project()
{
}