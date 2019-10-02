#include "printscreen.hpp"

GLvoid PrintScreen::post(GLboolean flush)
{
	if(flush)
	{
		glFlush();
	}

	if(GLEW_VERSION_3_0)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	else
	{
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	}

	glReadBuffer(GL_BACK);
	glDrawBuffer(GL_BACK);
}

PrintScreen::PrintScreen():
fbo(0),
rbo(0),
size(std::array<GLushort, 2>{0, 0})
{
	if(GLEW_VERSION_3_0)
	{
		glGenFramebuffers(1, &this->fbo);
		glGenRenderbuffers(1, &this->rbo);
	}
	else
	{
		glGenFramebuffersEXT(1, &this->fbo);
		glGenRenderbuffersEXT(1, &this->rbo);
	}
}

PrintScreen::~PrintScreen()
{
	if(GLEW_VERSION_3_0)
	{
		if(this->fbo > 0)
		{
			glDeleteFramebuffers(1, &this->fbo);
		}

		if(this->rbo > 0)
		{
			glDeleteRenderbuffers(1, &this->rbo);
		}
	}
	else
	{
		if(this->fbo > 0)
		{
			glDeleteFramebuffersEXT(1, &this->fbo);
		}

		if(this->rbo > 0)
		{
			glDeleteRenderbuffersEXT(1, &this->rbo);
		}
	}
}

GLvoid PrintScreen::createRenderBuffer(std::array<GLushort, 2> size)
{
	this->size = size;

	if(GLEW_VERSION_3_0)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
		glBindRenderbuffer(GL_RENDERBUFFER, this->rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, this->size[mSIZE::WIDTH], this->size[mSIZE::HEIGHT]);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, this->rbo);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	else
	{
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, this->fbo);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, this->rbo);
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_RGBA8_EXT, this->size[mSIZE::WIDTH], this->size[mSIZE::HEIGHT]);
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_RENDERBUFFER_EXT, this->rbo);

		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	}
}

GLvoid PrintScreen::saveScreen()
{
	GLushort width = this->size[mSIZE::WIDTH];
	GLushort height = this->size[mSIZE::HEIGHT];

	if(GLEW_VERSION_3_0)
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->fbo);
		glReadBuffer(GL_FRONT);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);

		glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}
	else
	{
		glBindFramebufferEXT(GL_READ_FRAMEBUFFER_EXT, 0);
		glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, this->fbo);
		glReadBuffer(GL_FRONT_AND_BACK);
		glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);

		glBlitFramebufferEXT(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}
}

GLvoid PrintScreen::loadScreen()
{
	GLushort width = this->size[mSIZE::WIDTH];
	GLushort height = this->size[mSIZE::HEIGHT];

	if(GLEW_VERSION_3_0)
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, this->fbo);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glReadBuffer(GL_COLOR_ATTACHMENT0);
		glDrawBuffer(GL_FRONT);

		glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}
	else
	{
		glBindFramebufferEXT(GL_READ_FRAMEBUFFER_EXT, this->fbo);
		glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, 0);
		glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
		glDrawBuffer(GL_FRONT_AND_BACK);

		glBlitFramebufferEXT(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}
}