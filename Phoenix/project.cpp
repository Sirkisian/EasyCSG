#include "project.hpp"

Project::Project()
{
	this->graphicWorld.addSceneAxis(9.0f);
	this->graphicWorld.addSceneGrid(36);
	this->graphicWorld.addTransformationAxis(50.0f, 20.0f, 30.0f);

	this->addDefaultObjects();
}

Project::~Project()
{
}

void Project::save(_IN_(std::basic_string<TCHAR> & fileName))
{
	std::basic_ofstream<TCHAR> file(fileName);

	if(file.is_open())
	{
		file << _T("<?xml version='1.0' encoding='utf-16'?>");
		FileIO::Export::push(file, _T("project"));
		this->graphicWorld.save(file);
		this->csgWorld.save(file);
		FileIO::Export::pop(file);
	}

	file.close();
}

void Project::reset()
{
	this->graphicWorld.clear();
	this->graphicWorld.drawSceneAxis = true;
	this->graphicWorld.drawSceneGrid = true;
	this->graphicWorld.setDrawMode(SceneObject::DRAWMODE::COLOR);
	this->graphicWorld.camera.setDefaultValues();

	this->csgWorld.clear();

	this->addDefaultObjects();
}

void Project::addDefaultObjects()
{
	std::array<float, 4> light{1.0f, 1.0f, 1.0f, 1.0f};
	this->graphicWorld.addGraphicLight(GL_LIGHT0);
	this->graphicWorld.setLightValues(GL_LIGHT0, light, Light::LIGHTTYPE::AMBIENT);
	this->graphicWorld.setLightValues(GL_LIGHT0, light, Light::LIGHTTYPE::DIFFUSE);
	this->graphicWorld.setLightValues(GL_LIGHT0, light, Light::LIGHTTYPE::SPECULAR);

	this->csgWorld.addCsgTree(_T("default"));
}