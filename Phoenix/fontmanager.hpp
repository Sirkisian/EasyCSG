#pragma once
//-----
#include <map>
#include <fstream>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "graphicfunctions.hpp"
#include "vertexshader.hpp"
#include "fragmentshader.hpp"
#include "shaderprogram.hpp"
//----
//---
//--
class Character
{
	public:
		~Character();

		GLuint texture;
		std::array<GLuint, 2> size;
		std::array<GLint, 2> bearing;
		GLuint advance;
};

class Font
{
	public:
		~Font();

		std::map<TCHAR, Character*> characters;
};

class FontManager : public DrawnObject
{
	public:
		FontManager();
		~FontManager();

		GLvoid setOrthoProjection(std::vector<GLfloat> projection);

		GLvoid loadFont(_IN_(std::basic_string<TCHAR> & fontPath));

		GLvoid drawText(std::basic_string<TCHAR> fontName, std::basic_string<TCHAR> text, GLfloat x, GLfloat y, GLfloat scale, std::array<GLfloat, 3> color);

	private:
		GLvoid bindVbo();

		GLboolean loadFontFile(_IN_(std::basic_string<TCHAR> & fontPath), _OUT_(std::vector<GLchar> & content));

		GLvoid loadCharacters(_IN_(FT_Face face), std::map<TCHAR, Character*> & characters);

		std::map<std::basic_string<TCHAR>, Font> fonts;

		ShaderProgram shaderProgram;
};