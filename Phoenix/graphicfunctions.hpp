#pragma once
//-----
#include "graphicobject.hpp"
//----
//---
//--
namespace GraphicFunctions
{
	namespace //needed for linking
	{
		GLvoid useShader(_IN_(GLuint & shader))
		{
			GLint program;
			glGetIntegerv(GL_CURRENT_PROGRAM, &program);

			if(shader != program)
			{
				glUseProgram(shader);
			}
		}

		GLvoid clearScene(_IN_(ARRAY4REF(GLfloat, clearColor)))
		{
			glClearColor(clearColor[mCOLOR::R], clearColor[mCOLOR::G], clearColor[mCOLOR::B], clearColor[mCOLOR::A]);
			glClearDepth(1);
			glClearStencil(0);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		}

		GLvoid beginAntiAliasing(GLboolean execute)
		{
			if(execute)
			{
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				glEnable(GL_LINE_SMOOTH);
				glEnable(GL_POLYGON_SMOOTH);
				glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
				glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
			}
		}

		GLvoid endAntiAliasing(GLboolean execute)
		{
			if(execute)
			{
				glDisable(GL_POLYGON_SMOOTH);
				glDisable(GL_LINE_SMOOTH);
				glDisable(GL_BLEND);
			}
		}

		GLvoid beginDrawFront()
		{
			if(GLEW_VERSION_3_0)
			{
				glDrawBuffer(GL_FRONT);
			}
			else
			{
				glDrawBuffer(GL_FRONT_AND_BACK); // in OpenGL 2.1 works only GL_FRONT_AND_BACK
			}
		}

		GLvoid endDrawFront()
		{
			glFlush();

			glDrawBuffer(GL_BACK);
		}

		template <class T>
		GLubyte selectObject(_IN_(long & x), _IN_(long & y), _IN_(GLuint & shader), _IN_(std::vector<T> & objects))
		{
			//max 255 objects with 8bit
			GLubyte selected = 0;

			size_t size = objects.size();

			if(size > 0 && size <= 255)
			{
				glClearDepth(1);
				glClearStencil(0);

				glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

				glEnable(GL_DEPTH_TEST);
				glEnable(GL_STENCIL_TEST);
				glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

				GLubyte index = 1;
				for(class std::vector<T>::const_iterator i = objects.begin(), j = objects.end(); i != j; i++)
				{
					glStencilFunc(GL_ALWAYS, index, -1);

					(*i)->drawObject(shader, SceneObject::DRAWMODE::SELECT);

					index++;
				}

				glDisable(GL_STENCIL_TEST);
				glDisable(GL_DEPTH_TEST);

				glReadPixels(x, y, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, &selected);

				//set back to default
				glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
			}

			return selected;
		}
	}
};