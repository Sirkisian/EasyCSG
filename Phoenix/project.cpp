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

		FileIO::Export::push(file, _T("graphic"));
		this->graphicWorld.save(file);
		FileIO::Export::pop(file);

		FileIO::Export::push(file, _T("csg"));
		this->csgWorld.save(file);
		FileIO::Export::pop(file);

		FileIO::Export::pop(file);
	}

	file.close();
}

void Project::load(_IN_(std::basic_string<TCHAR> & fileName))
{
	std::basic_ifstream<TCHAR> file(fileName);

	if(file.is_open())
	{
		bool error = false;
		rapidxml::file<TCHAR> xmlFile(file);
		rapidxml::xml_document<TCHAR> document;

		try
		{
			document.parse<0>(xmlFile.data());
		}
		catch(rapidxml::parse_error &)
		{
			error = true;
		}

		if(!error)
		{
			rapidxml::xml_node<TCHAR>* rootNode = document.first_node();

			if(FileIO::Import::isTag(_T("project"), rootNode))
			{
				rapidxml::xml_node<TCHAR>* node = rootNode->first_node();

				while(node != nullptr)
				{
					if(FileIO::Import::isTag(_T("graphic"), node))
					{
						this->graphicWorld.load(node);

						rapidxml::xml_node<TCHAR>* childNode = node->first_node(_T("objects"));

						if(childNode != nullptr)
						{
							std::vector<GraphicObject*> objects;

							this->objectManager.load(objects, childNode);

							for(std::vector<GraphicObject*>::const_iterator i = objects.begin(), j = objects.end(); i != j; i++) 
							{
								this->graphicWorld.addGraphicObject(*i);
							}
						}
					}
					else if(FileIO::Import::isTag(_T("csg"), node))
					{
						rapidxml::xml_node<TCHAR>* childNode = node->first_node(_T("trees"));

						if(childNode != nullptr)
						{
							this->csgWorld.load(childNode, this->graphicWorld);
						}
					}

					node = node->next_sibling();
				}
			}
		}
	}

	file.close();
}

void Project::reset()
{
	this->csgWorld.clear();

	this->graphicWorld.clear();
	this->graphicWorld.drawSceneAxis = true;
	this->graphicWorld.drawSceneGrid = true;
	this->graphicWorld.setDrawMode(SceneObject::DRAWMODE::COLOR);
	this->graphicWorld.camera.setDefaultValues();

	this->objectManager.reset();

	this->textureManager.clear();

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