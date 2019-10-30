#include "graphicworld.hpp"

const std::array<GLfloat, 4> GraphicWorld::sceneColor{0.85f, 0.85f, 0.85f, 1.0f};

GraphicWorld::GraphicWorld():
sceneAxis(nullptr),
sceneGrid(nullptr),
transformationAxis(nullptr),
drawSceneAxis(GL_TRUE),
drawSceneGrid(GL_TRUE),
drawTransformationAxis(GL_FALSE),
mode(SceneObject::DRAWMODE::COLOR)
{
	this->perspective = glm::mat4(1.0f); //identity matrix
	this->orthographic = glm::mat4(1.0f);

	std::basic_string<GLchar> vertexCode = VertexShader::getVertexShader(VertexShader::TYPE::SHADERV);
	std::basic_string<GLchar> fragmentCode = FragmentShader::getFragmentShader(FragmentShader::TYPE::SHADERF);
	this->shaderProgram.addShaders(vertexCode, fragmentCode);

	vertexCode = VertexShader::getVertexShader(VertexShader::TYPE::FIXEDV);
	fragmentCode = FragmentShader::getFragmentShader(FragmentShader::TYPE::FIXEDF);
	this->shaderProgramFixed.addShaders(vertexCode, fragmentCode);

	this->lights.fill(nullptr);
}

GraphicWorld::~GraphicWorld()
{
	if(this->sceneAxis != nullptr)
	{
		delete this->sceneAxis;
		this->sceneAxis = nullptr; 
	}

	if(this->sceneGrid != nullptr)
	{
		delete this->sceneGrid;
		this->sceneGrid = nullptr;
	}

	if(this->transformationAxis != nullptr)
	{
		delete this->transformationAxis;
		this->transformationAxis = nullptr;
	}

	this->clear();
}

GLvoid GraphicWorld::setTransformationAxisPosition(long x, long y)
{
	if(this->transformationAxis != nullptr)
	{
		this->transformationAxis->translateTransformationAxis(x, y);
	}
}

GLvoid GraphicWorld::setProjection(GLfloat degree, GLfloat zNear, GLfloat zFar, GLushort windowWidth, GLushort windowHeight)
{
	glViewport(0, 0, windowWidth, windowHeight);

	this->perspective = glm::perspective(glm::radians(degree), static_cast<GLfloat>(windowWidth) / static_cast<GLfloat>(windowHeight), zNear, zFar);

	this->orthographic = glm::ortho(0.0f, static_cast<GLfloat>(windowWidth), 0.0f, static_cast<GLfloat>(windowHeight), -1.0f, 10.0f); //bottom left corner is x:0, y:0

	//for OpenCSG begin
	glUseProgram(0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMultMatrixf(glm::value_ptr(this->perspective));
	//for OpenCSG end
}

GLvoid GraphicWorld::drawScene(GLboolean antiAlias)
{
	const GLuint & shader = this->shaderProgram.useShaderProgram();

	GraphicFunctions::clearScene(GraphicWorld::sceneColor);

	this->beginDraw(shader, antiAlias);

	this->drawObjects(shader, antiAlias);

	this->endDraw(shader);

	this->drawAxis(shader, antiAlias);
}

GLvoid GraphicWorld::beginDraw(_IN_(GLuint & shader), GLboolean antiAlias, GLboolean fixedPipeline)
{
	GraphicFunctions::useShader(shader);

	this->setShaderVPVar(shader);

	//for OpenCSG begin
	if(this->mode == SceneObject::DRAWMODE::CSG)
	{
		if(fixedPipeline)
		{
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glm::mat4 camera(1.0f);
			this->camera.multViewMatrix(camera);
			glMultMatrixf(glm::value_ptr(camera));
		}
	}
	//for OpenCSG end

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);

	ShaderProgram::setBool(shader, std::basic_string<GLchar>("lighting"), GL_FALSE);

	if(this->mode != SceneObject::DRAWMODE::CSG)
	{
		if(this->drawSceneAxis && this->sceneAxis != nullptr)
		{
			GraphicFunctions::beginAntiAliasing(antiAlias);
			this->sceneAxis->drawObject(shader);
			GraphicFunctions::endAntiAliasing(antiAlias);
		}

		if(this->drawSceneGrid && this->sceneGrid != nullptr)
		{
			GraphicFunctions::beginAntiAliasing(antiAlias);
			this->sceneGrid->drawObject(shader);
			GraphicFunctions::endAntiAliasing(antiAlias);
		}
	}

	if(this->mode == SceneObject::DRAWMODE::LIGHTING || this->mode == SceneObject::DRAWMODE::TEXTURE || this->mode == SceneObject::DRAWMODE::CSG)
	{
		ShaderProgram::setBool(shader, std::basic_string<GLchar>("lighting"), GL_TRUE);

		const glm::vec3 & position = this->camera.getCameraPosition();

		std::array<GLfloat, 3> cameraPosition{position.x, position.y, position.z};

		ShaderProgram::setVector<3>(shader, std::basic_string<GLchar>("viewPosition"), &cameraPosition);

		for(std::array<GraphicLight*, 8>::const_iterator i = this->lights.begin(), j = this->lights.end(); i != j; i++)
		{
			if(*i != nullptr)
			{
				ShaderProgram::setVector<3>(shader, std::basic_string<GLchar>("light.position"), (*i)->light.getLightPosition());
				ShaderProgram::setVector<4>(shader, std::basic_string<GLchar>("light.ambient"), (*i)->light.getLightValues(Light::LIGHTTYPE::AMBIENT));
				ShaderProgram::setVector<4>(shader, std::basic_string<GLchar>("light.diffuse"), (*i)->light.getLightValues(Light::LIGHTTYPE::DIFFUSE));
				ShaderProgram::setVector<4>(shader, std::basic_string<GLchar>("light.specular"), (*i)->light.getLightValues(Light::LIGHTTYPE::SPECULAR));

				if(this->mode != SceneObject::DRAWMODE::CSG)
				{
					if((*i)->lightCube != nullptr)
					{
						(*i)->lightCube->drawObject(shader, SceneObject::DRAWMODE::LIGHTING);
					}
				}
			}
		}
	}
}

GLvoid GraphicWorld::endDraw(_IN_(GLuint & shader))
{
	if(this->mode == SceneObject::DRAWMODE::LIGHTING || this->mode == SceneObject::DRAWMODE::TEXTURE || this->mode == SceneObject::DRAWMODE::CSG)
	{
		GraphicFunctions::useShader(shader);

		ShaderProgram::setBool(shader, std::basic_string<GLchar>("lighting"), GL_FALSE);
	}

	glDisable(GL_STENCIL_TEST);
	glDisable(GL_DEPTH_TEST);
}

GLvoid GraphicWorld::drawAxis(_IN_(GLuint & shader), GLboolean antiAlias)
{
	//Draw 2D
	if(this->drawTransformationAxis && this->transformationAxis != nullptr)
	{
		GraphicFunctions::useShader(shader);

		this->setShaderVPOrthoVar(shader);

		glEnable(GL_DEPTH_TEST);

		GraphicFunctions::beginAntiAliasing(antiAlias);
		this->transformationAxis->drawTransformationAxis(shader);
		GraphicFunctions::endAntiAliasing(antiAlias);

		glDisable(GL_DEPTH_TEST);
	}
}

GLvoid GraphicWorld::addTransformationAxis(GLfloat axisLength, GLfloat triangleSize, GLfloat cubeSize)
{
	this->transformationAxis = new TransformationAxis(axisLength, triangleSize, cubeSize);

	if(this->transformationAxis != nullptr && this->transformationAxis->cube != nullptr)
	{
		this->transformationAxis->cube->setObjectColor(GraphicObject::selectedObjectColor);
	}
}

GLvoid GraphicWorld::addGraphicLight(GLushort lightSource)
{
	GLbyte arrayPosition = this->getLightArrayPosition(lightSource);

	if(arrayPosition >= 0)
	{
		if(this->lights[arrayPosition] == nullptr)
		{
			this->lights[arrayPosition] = new GraphicLight(lightSource);
		}
	}
}

GLvoid GraphicWorld::deleteGraphicObject(GLuint id)
{
	std::vector<GraphicObject*>::iterator location = std::find_if(this->selectedObjects.begin(), this->selectedObjects.end(), ComparatorGraphicObject(id));

	if(location != this->selectedObjects.end())
	{
		this->selectedObjects.erase(location);
	}

	location = std::find_if(this->objects.begin(), this->objects.end(), ComparatorGraphicObject(id));

	if(location != this->objects.end())
	{
		delete *location;

		this->objects.erase(location);
	}
}

size_t GraphicWorld::selectObject(long x, long y)
{
	const GLuint & shader = this->shaderProgram.useShaderProgram();

	this->setShaderVPVar(shader);

	return GraphicFunctions::selectObject(x, y, shader, this->objects);
}

size_t GraphicWorld::selectObjects(long x, long y)
{
	size_t counter = 0;

	size_t position = this->selectObject(x, y);

	if(position > 0)
	{
		GraphicObject* object = GraphicWorld::getObject(position - 1, this->objects);

		if(object != nullptr)
		{
			GLboolean selected = GL_FALSE;

			for(std::vector<GraphicObject*>::const_iterator i = this->selectedObjects.begin(), j = this->selectedObjects.end(); i != j; i++)
			{
				counter++;

				if(*i == object)
				{
					selected = GL_TRUE;

					break;
				}
			}

			if(!selected)
			{
				object->setObjectColor(GraphicObject::selectedObjectColor);

				ExceptionHandler::push_back<GraphicObject*>(this->selectedObjects, object);

				counter++;
			}
		}
	}

	return counter;
}

GLvoid GraphicWorld::clearSelection()
{
	if(!this->selectedObjects.empty())
	{
		for(std::vector<GraphicObject*>::const_iterator i = this->selectedObjects.begin(), j = this->selectedObjects.end(); i != j; i++)
		{
			(*i)->setObjectColor(GraphicObject::defaultObjectColor);
		}

		this->selectedObjects.clear();
	}
}

GLboolean GraphicWorld::selectTransformationAxis(long x, long y, _OUT_(mCOORDINATE & axis))
{
	if(this->transformationAxis != nullptr && this->transformationAxis->triangleAxis != nullptr)
	{
		const GLuint & shader = this->shaderProgram.useShaderProgram();

		this->setShaderVPOrthoVar(shader);

		std::vector<SceneObject*> triangleAxis;
		ExceptionHandler::push_back<SceneObject*>(triangleAxis, this->transformationAxis->triangleAxis);

		size_t position = GraphicFunctions::selectObject(x, y, shader, triangleAxis);

		if(position > 0)
		{
			std::array<GLubyte, 3> color;

			glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, color.data());

			if(color[0] == 255)
			{
				axis = mCOORDINATE::X;
			}
			else if(color[1] == 255)
			{
				axis = mCOORDINATE::Y;
			}
			else if(color[2] == 255)
			{
				axis = mCOORDINATE::Z;
			}
			else
			{
				return GL_FALSE;
			}

			return GL_TRUE;
		}
	}

	return GL_FALSE;
}

GLboolean GraphicWorld::objectNameExists(_IN_(std::basic_string<TCHAR> & objectName))
{
	for(std::vector<GraphicObject*>::const_iterator i = this->objects.begin(), j = this->objects.end(); i != j; i++)
	{
		if((*i)->getObjectName().compare(objectName) == 0)
		{
			return GL_TRUE;
		}
	}

	return GL_FALSE;
}

GLvoid GraphicWorld::transformSelectedObjects(_IN_(mCOORDINATE & axis), _IN_(Transformation::TYPE & type), _IN_(GLfloat & value))
{
	std::array<GLfloat, 3> arguments{0.0f, 0.0f, 0.0f};

	arguments[axis] = value;

	for(std::vector<GraphicObject*>::const_iterator i = this->selectedObjects.begin(), j = this->selectedObjects.end(); i != j; i++)
	{
		switch(type)
		{
			case Transformation::TYPE::TRANSLATE:
				{
					(*i)->transformation.translate(arguments[0], arguments[1], arguments[2], Transformation::ACTION::ADD);
				}
				break;

			case Transformation::TYPE::SCALE:
				{
					(*i)->transformation.scale(arguments[0], arguments[1], arguments[2], Transformation::ACTION::ADD);
				}
				break;

			case Transformation::TYPE::ROTATE:
				{
					(*i)->transformation.rotate(arguments[0], arguments[1], arguments[2], Transformation::ACTION::ADD);
				}
				break;
		}
	}
}

GLvoid GraphicWorld::setLightPosition(GLushort lightSource, _IN_(ARRAY3REF(GLfloat, position)))
{
	GLbyte arrayPosition = this->getLightArrayPosition(lightSource);

	if(arrayPosition >= 0)
	{
		if(this->lights[arrayPosition] != nullptr)
		{
			this->lights[arrayPosition]->setPosition(position);
		}
	}
}

GLvoid GraphicWorld::setLightValues(GLushort lightSource, _IN_(ARRAY4REF(GLfloat, values)), Light::LIGHTTYPE lightType)
{
	GLbyte arrayPosition = this->getLightArrayPosition(lightSource);

	if(arrayPosition >= 0)
	{
		if(this->lights[arrayPosition] != nullptr)
		{
			this->lights[arrayPosition]->light.setLightValues(values, lightType);
		}
	}
}

GLvoid GraphicWorld::setMaterialValues(_IN_(ARRAY4REF(GLfloat, values)), Material::MATERIALTYPE materialType)
{
	for(std::vector<GraphicObject*>::const_iterator i = this->selectedObjects.begin(), j = this->selectedObjects.end(); i != j; i++)
	{
		(*i)->material.setMaterialValues(values, materialType);
	}
}

const std::array<GLfloat, 4>* GraphicWorld::getMaterialValues(size_t position, Material::MATERIALTYPE materialType)
{
	GraphicObject* object = GraphicWorld::getObject(position - 1, this->selectedObjects);

	if(object != nullptr)
	{
		return object->material.getMaterialValues(materialType);
	}

	return nullptr;
}

GLvoid GraphicWorld::setTexture(GLuint texture)
{
	for(std::vector<GraphicObject*>::const_iterator i = this->selectedObjects.begin(), j = this->selectedObjects.end(); i != j; i++)
	{
		GraphicObjectG* object = dynamic_cast<GraphicObjectG*>(*i);

		if(object != nullptr)
		{
			object->setTexture(texture);
		}
	}
}

GLvoid GraphicWorld::centerObject(size_t position)
{
	GraphicObject* object = GraphicWorld::getObject(position - 1, this->objects);

	if(object == nullptr)
	{
		this->camera.setCameraDirection(glm::vec3(0.0f));
	}
	else
	{
		this->camera.setCameraDirection(object->transformation.getTranslation());
	}
}

GLvoid GraphicWorld::save(std::basic_ofstream<TCHAR> & file)
{
	FileIO::Export::push(file, _T("camera"));
	file << this->camera << FileIO::Export::pop;

	FileIO::Export::push(file, _T("lights"));
	for(std::array<GraphicLight*, 8>::const_iterator i = this->lights.begin(), j = this->lights.end(); i != j; i++)
	{
		if(*i != nullptr)
		{
			FileIO::Export::push(file, _T("light"));
			file << *(*i) << FileIO::Export::pop;
		}
	}
	FileIO::Export::pop(file);

	FileIO::Export::push(file, _T("objects"));
	for(std::vector<GraphicObject*>::const_iterator i = this->objects.begin(), j = this->objects.end(); i != j; i++)
	{
		FileIO::Export::push(file, _T("object"));
		file << *i << FileIO::Export::pop;
	}
	FileIO::Export::pop(file);
}

GLvoid GraphicWorld::clear()
{
	this->selectedObjects.clear();

	if(!this->objects.empty())
	{
		for(std::vector<GraphicObject*>::const_iterator i = this->objects.begin(), j = this->objects.end(); i != j; i++)
		{
			delete *i;
		}

		this->objects.clear();
	}

	for(std::array<GraphicLight*, 8>::iterator i = this->lights.begin(), j = this->lights.end(); i != j; i++)
	{
		delete *i;

		*i = nullptr;
	}
}

const std::map<GLushort, GLubyte> GraphicWorld::lightSources{{GL_LIGHT0, 0}, {GL_LIGHT1, 1}, {GL_LIGHT2, 2}, {GL_LIGHT3, 3}, {GL_LIGHT4, 4}, {GL_LIGHT5, 5}, {GL_LIGHT6, 6}, {GL_LIGHT7, 7}};

GraphicObject* GraphicWorld::getObject(size_t position, _IN_(std::vector<GraphicObject*> & objects))
{
	GraphicObject* object = nullptr;

	try
	{
		object = objects.at(position);
	}
	catch(const std::out_of_range &)
	{
	}

	return object;
}

GLvoid GraphicWorld::getViewProjectionMatrix(_OUT_(glm::mat4 & viewProjection))
{
	viewProjection = this->perspective;
	this->camera.multViewMatrix(viewProjection);
}

GLvoid GraphicWorld::drawObjects(_IN_(GLuint & shader), GLboolean antiAlias)
{
	antiAlias = antiAlias && this->mode == SceneObject::DRAWMODE::WIRE || this->mode == SceneObject::DRAWMODE::COLOR;

	GraphicFunctions::beginAntiAliasing(antiAlias);

	for(std::vector<GraphicObject*>::const_iterator i = this->objects.begin(), j = this->objects.end(); i != j; i++) 
	{
		(*i)->drawObject(shader, this->mode);
	}

	GraphicFunctions::endAntiAliasing(antiAlias);
}

GLvoid GraphicWorld::setShaderVPVar(_IN_(GLuint & shader))
{
	glm::mat4 viewProjection = glm::mat4(1.0f);
	this->getViewProjectionMatrix(viewProjection);

	this->setShaderVPVar(shader, viewProjection);
}

GLvoid GraphicWorld::setShaderVPVar(_IN_(GLuint & shader), _IN_(glm::mat4 & viewProjection))
{
	const GLfloat* pFloat = glm::value_ptr(viewProjection);
	std::vector<GLfloat> matrix(pFloat, pFloat + 16);

	ShaderProgram::setMatrix(shader, std::basic_string<GLchar>("viewProjection"), matrix);
}

GLbyte GraphicWorld::getLightArrayPosition(_IN_(GLushort & lightSource))
{
	GLbyte position;

	try
	{
		position = GraphicWorld::lightSources.at(lightSource);
	}
	catch(const std::out_of_range &)
	{
		position = -1;
	}

	return position;
}