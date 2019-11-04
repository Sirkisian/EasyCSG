#pragma once
//-----
#include <vector>
#include <gl/glew.h>
#include <tchar.h>
#include "macros.hpp"
#include "exceptionhandler.hpp"
//----
//---
//--
class Texture
{
	public:
		Texture():
		id(0)
		{
		}

		GLuint id;
		std::basic_string<TCHAR> file;
};

class TextureManager
{
	public:
		TextureManager();
		~TextureManager();

		GLuint textureLoaded(_IN_(std::basic_string<TCHAR> & file));

		GLuint loadTexture(_IN_(std::basic_string<TCHAR> & file));

		GLvoid clear();

	private:
		std::vector<Texture> textures;
};