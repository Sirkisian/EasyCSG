#pragma once
//-----
#include <gl/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "macros.hpp"
#include "enumconverter.hpp"
//----
//---
//--
class Transformation
{
	public:
		enum class ACTION : GLubyte {ADD, SET};
		enum class TYPE : GLubyte {NOTYPE, TRANSLATE, SCALE, ROTATE};

		static inline Transformation::TYPE convertType(GLubyte type)
		{
			return EnumConverter::convert<Transformation::TYPE>(type, Transformation::TYPE::TRANSLATE, Transformation::TYPE::ROTATE);
		}

		Transformation();
		~Transformation();

		GLvoid multModelMatrix(_INOUT_(glm::mat4 & matrix)) const;

		inline GLvoid translate(GLfloat x, GLfloat y, GLfloat z, Transformation::ACTION action)
		{
			this->transform(this->translation, x, y, z, action);
		}

		inline GLvoid scale(GLfloat x, GLfloat y, GLfloat z, Transformation::ACTION action)
		{
			this->transform(this->scaling, x, y, z, action);
		}

		inline GLvoid rotate(GLfloat angleX, GLfloat angleY, GLfloat angleZ, Transformation::ACTION action)
		{
			this->transform(this->rotation, angleX, angleY, angleZ, action);
		}

		inline glm::vec3 getTranslation()
		{
			return this->translation;
		}

	private:
		GLvoid transform(_INOUT_(glm::vec3 & transformation), _IN_(GLfloat & x), _IN_(GLfloat & y), _IN_(GLfloat & z), _IN_(Transformation::ACTION & action));

		glm::vec3 translation;
		glm::vec3 scaling;
		glm::vec3 rotation;
};