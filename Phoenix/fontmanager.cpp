#include "fontmanager.hpp"

Character::~Character()
{
	if(this->texture > 0)
	{
		if(glIsTexture(this->texture))
		{
			glDeleteTextures(1, &this->texture);
		}
	}
}

Font::~Font()
{
	for(std::map<TCHAR, Character*>::iterator i = this->characters.begin(), j = this->characters.end(); i != j; i++)
	{
		delete i->second;
	}

	this->characters.clear();
}

FontManager::FontManager()
{
	this->shaderProgram.addShaders(VertexShader::codeFontVertex, FragmentShader::codeFontFragment);
}

FontManager::~FontManager()
{
}

GLvoid FontManager::setOrthoProjection(std::vector<GLfloat> projection)
{
	GLuint shader = this->shaderProgram.useShaderProgram();

	ShaderProgram::setMatrix(shader, std::basic_string<GLchar>("projection"), projection);
}

GLvoid FontManager::loadFont(_IN_(std::basic_string<TCHAR> & fontPath))
{
	size_t position = fontPath.find_last_of(_T("\\"));

	if(position > 0)
	{
		Font font;

		std::basic_string<TCHAR> fontName = fontPath.substr(position + 1);

		fontName = fontName.substr(0, fontName.length() - 4); // ".ttf"

		std::pair<std::map<std::basic_string<TCHAR>, Font>::iterator, GLboolean> inserted = this->fonts.insert({fontName, font});

		if(inserted.second)
		{
			std::vector<GLchar> buffer;
			if(this->loadFontFile(fontPath, buffer))
			{
				FT_Face face = nullptr;
				FT_Library library = nullptr;

				if(FT_Init_FreeType(&library) == 0)
				{
					if(FT_New_Memory_Face(library, reinterpret_cast<FT_Byte*>(buffer.data()), buffer.size(), 0, &face) == 0)
					{
						if(FT_Set_Pixel_Sizes(face, 0, 24) == 0)
						{
							this->loadCharacters(face, inserted.first->second.characters);
						}

						FT_Done_Face(face);
					}

					FT_Done_FreeType(library);
				}
			}
		}
	}
}

GLvoid FontManager::drawText(std::basic_string<TCHAR> fontName, std::basic_string<TCHAR> text, GLfloat x, GLfloat y, GLfloat scale, std::array<GLfloat, 3> color)
{
	std::map<std::basic_string<TCHAR>, Font>::const_iterator font = this->fonts.find(fontName);

	if(font != this->fonts.end())
	{
		const std::map<TCHAR, Character*> & characters = font->second.characters;

		GLuint shader = this->shaderProgram.useShaderProgram();

		ShaderProgram::setVector(shader, std::basic_string<GLchar>("color"), &color);

		glActiveTexture(GL_TEXTURE0);

		GraphicFunctions::beginAntiAliasing(GL_TRUE);

		this->bindVao();
		this->bindVbo();

		GLfloat positionX = 0.0f, positionY = 0.0f, width = 0.0f, height = 0.0f;
		for(std::basic_string<TCHAR>::const_iterator i = text.begin(), j = text.end(); i != j; i++)
		{
			std::map<TCHAR, Character*>::const_iterator character = characters.find(*i);

			if(character != characters.end())
			{
				const Character* ch = character->second;

				positionX = x + ch->bearing[mCOORDINATE::X] * scale;
				positionY = y - (ch->size[mCOORDINATE::Y] - ch->bearing[mCOORDINATE::Y]) * scale;

				width = ch->size[mCOORDINATE::X] * scale;
				height = ch->size[mCOORDINATE::Y] * scale;

				GLfloat vertices[6][4] = {
				{positionX,         positionY + height, 0.0f, 0.0f},
				{positionX,         positionY,          0.0f, 1.0f},
				{positionX + width, positionY,          1.0f, 1.0f},

				{positionX,         positionY + height, 0.0f, 0.0f},
				{positionX + width, positionY,          1.0f, 1.0f},
				{positionX + width, positionY + height, 1.0f, 0.0f}};

				glBindTexture(GL_TEXTURE_2D, ch->texture);

				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 

				glPolygonMode(GL_FRONT, GL_FILL);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				x += (ch->advance >> 6) * scale;
			}
		}

		this->unbind();

		GraphicFunctions::endAntiAliasing(GL_TRUE);
	}
}

GLvoid FontManager::bindVbo()
{
	if(this->vbo > 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, 0);
		glEnableVertexAttribArray(0);
	}
}

GLboolean FontManager::loadFontFile(_IN_(std::basic_string<TCHAR> & fontPath), _OUT_(std::vector<GLchar> & content))
{
	std::basic_ifstream<GLchar> file(fontPath, std::ios::binary | std::ios::ate);

	if(file.is_open())
	{
		size_t size = file.tellg();
		file.seekg(0, std::ios::beg);

		try
		{
			content.resize(size);
		}
		catch(const std::exception &)
		{
			return GL_FALSE;
		}

		file.read(content.data(), size);

		file.close();

		return GL_TRUE;
	}

	return GL_FALSE;
}

GLvoid FontManager::loadCharacters(_IN_(FT_Face face), std::map<TCHAR, Character*> & characters)
{
	GLuint texture = 0;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for(GLubyte i = 32; i < 127; i++)
	{
		if(FT_Load_Char(face, i, FT_LOAD_RENDER) == 0)
		{
			FT_GlyphSlot glyph = face->glyph;

			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, glyph->bitmap.width, glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, glyph->bitmap.buffer);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			Character* character = new Character{texture, std::array<GLuint, 2>{glyph->bitmap.width, glyph->bitmap.rows}, std::array<GLint, 2>{glyph->bitmap_left, glyph->bitmap_top}, static_cast<GLuint>(glyph->advance.x)};

			if(character != nullptr)
			{
				characters.insert({i, character});
			}
		}
	}
}