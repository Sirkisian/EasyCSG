#pragma once
//-----
#include <map>
#include "exceptionhandler.hpp"
#include "camera.hpp"
#include "graphiclight.hpp"
#include "transformationaxis.hpp"
#include "vertexshader.hpp"
#include "fragmentshader.hpp"
#include "graphicfunctions.hpp"
//----
//---
//--
class ComparatorGraphicObject
{
	public:
		ComparatorGraphicObject(GLuint id):
		id(id)
		{
		}

		GLboolean operator()(_IN_(GraphicObject* object))
		{
			return object->getObjectId() == this->id;
		}

	private:
		GLuint id;
};

class GraphicWorld
{
	public:
		static const std::array<GLfloat, 4> sceneColor;

		GraphicWorld();
		~GraphicWorld();

		inline GLvoid setDrawMode(SceneObject::DRAWMODE mode)
		{
			this->mode = mode;
		}

		inline SceneObject::DRAWMODE getDrawMode()
		{
			return this->mode;
		}

		GLvoid setTransformationAxisPosition(long x, long y);
		GLvoid setProjection(GLfloat degree, GLfloat zNear, GLfloat zFar, GLushort windowWidth, GLushort windowHeight);

		inline std::vector<GLfloat> getOrthoProjection()
		{
			const GLfloat* pFloat = glm::value_ptr(this->orthographic);

			return std::vector<GLfloat>(pFloat, pFloat + 16);
		}

		GLvoid drawScene(GLboolean antiAlias);
		GLvoid beginDraw(_IN_(GLuint & shader), GLboolean antiAlias, GLboolean fixedPipeline = GL_FALSE);
		GLvoid endDraw(_IN_(GLuint & shader));
		GLvoid drawAxis(_IN_(GLuint & shader), GLboolean antiAlias);

		inline GLvoid addSceneAxis(GLfloat length)
		{
			this->sceneAxis = ObjectManager::createAxis(length, ObjectManager::DIMENSION::Object3D);
		}

		inline GLvoid addSceneGrid(GLubyte lineCount)
		{
			this->sceneGrid = ObjectManager::createGrid(lineCount);
		}

		GLvoid addTransformationAxis(GLfloat axisLength, GLfloat triangleSize, GLfloat cubeSize);
		GLvoid addGraphicLight(GLushort lightSource);

		inline GLvoid addGraphicObject(GraphicObject* object)
		{
			ExceptionHandler::push_back<GraphicObject*>(this->objects, object);
		}

		GLvoid deleteGraphicObject(GLuint id);

		size_t selectObject(long x, long y);
		size_t selectObjects(long x, long y);
		GLvoid clearSelection();

		inline size_t selectedObjectsCount()
		{
			return this->selectedObjects.size();
		}

		GLboolean selectTransformationAxis(long x, long y, _OUT_(mCOORDINATE & axis));

		GLboolean objectNameExists(_IN_(std::basic_string<TCHAR> & objectName));

		GLvoid transformSelectedObjects(_IN_(mCOORDINATE & axis), _IN_(Transformation::TYPE & type), _IN_(GLfloat & value));

		GLvoid setLightPosition(GLushort lightSource, _IN_(ARRAY3REF(GLfloat, position)));
		GLvoid setLightValues(GLushort lightSource, _IN_(ARRAY4REF(GLfloat, values)), Light::LIGHTTYPE lightType);
		GLvoid setMaterialValues(_IN_(ARRAY4REF(GLfloat, values)), Material::MATERIALTYPE materialType);
		const std::array<GLfloat, 4>* getMaterialValues(size_t position, Material::MATERIALTYPE materialType);

		GLvoid setTexture(GLuint texture);

		GLvoid centerObject(size_t position);

		const std::vector<GraphicObject*> & getSelectedObjects()
		{
			return this->selectedObjects;
		}

		inline const GLuint & getShaderProgram()
		{
			return this->shaderProgram.getShaderProgram();
		}

		inline const GLuint & getShaderProgramFixed()
		{
			return this->shaderProgramFixed.getShaderProgram();
		}

		GLboolean drawSceneAxis;
		GLboolean drawSceneGrid;
		GLboolean drawTransformationAxis;

		Camera camera;

		ObjectManager objectManager;

	private:
		static const std::map<GLushort, GLubyte> lightSources;

		static GraphicObject* getObject(size_t position, _IN_(std::vector<GraphicObject*> & objects));

		GLvoid getViewProjectionMatrix(_OUT_(glm::mat4 & viewProjection));

		GLvoid drawObjects(_IN_(GLuint & shader), GLboolean antiAlias);

		GLvoid setShaderVPVar(_IN_(GLuint & shader));

		inline GLvoid setShaderVPOrthoVar(_IN_(GLuint & shader))
		{
			this->setShaderVPVar(shader, this->orthographic);
		}

		GLvoid setShaderVPVar(_IN_(GLuint & shader), _IN_(glm::mat4 & viewProjection));

		GLbyte getLightArrayPosition(_IN_(GLushort & lightSource));

		glm::mat4 perspective;
		glm::mat4 orthographic;

		ShaderProgram shaderProgram;
		ShaderProgram shaderProgramFixed;

		Axis* sceneAxis;
		Grid* sceneGrid;
		TransformationAxis* transformationAxis;

		SceneObject::DRAWMODE mode;

		std::vector<GraphicObject*> objects;
		std::vector<GraphicObject*> selectedObjects;

		std::array<GraphicLight*, 8> lights;
};