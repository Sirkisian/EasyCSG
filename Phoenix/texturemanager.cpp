#include "texturemanager.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
	this->clear();
}

GLuint TextureManager::textureLoaded(_IN_(std::basic_string<TCHAR> & file))
{
	for(std::vector<Texture>::const_iterator i = this->textures.begin(), j = this->textures.end(); i != j; i++) 
	{
		if(i->file.compare(file) == 0)
		{
			if(glIsTexture(i->id))
			{
				return i->id;
			}
		}
	}

	return 0;
}

GLuint TextureManager::loadTexture(_IN_(std::basic_string<TCHAR> & file))
{
	Texture texture;
	texture.file = file;

	FILE* image = nullptr;
	_tfopen_s(&image, texture.file.data(), _T("rb"));

	if(image != nullptr)
	{
		GLint width, height, nrChannels;

		stbi_set_flip_vertically_on_load(true);
		GLubyte* data = stbi_load_from_file(image, &width, &height, &nrChannels, STBI_rgb_alpha);

		if(data != nullptr)
		{
			glGenTextures(1, &texture.id);

			if(texture.id > 0)
			{
				glBindTexture(GL_TEXTURE_2D, texture.id); 

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				if(!GLEW_VERSION_3_0)
				{
					glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
				}

				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

				if(GLEW_VERSION_3_0)
				{
					glGenerateMipmap(GL_TEXTURE_2D);
				}

				ExceptionHandler::push_back<Texture>(this->textures, texture);
			}
		}

		stbi_image_free(data);

		fclose(image);
	}

	return texture.id;
}

GLvoid TextureManager::clear()
{
	for(std::vector<Texture>::const_iterator i = this->textures.begin(), j = this->textures.end(); i != j; i++)
	{
		if(glIsTexture(i->id))
		{
			glDeleteTextures(1, &(i->id));
		}
	}

	this->textures.clear();
}