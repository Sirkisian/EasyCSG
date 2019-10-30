#pragma once
//-----
#include "csgworld.hpp"
#include "graphicworld.hpp"
#include "texturemanager.hpp"
#include "fontmanager.hpp"
//----
//---
//--
class Project
{
	public:
		Project();
		~Project();

		void save(_IN_(std::basic_string<TCHAR> & fileName));

		void reset();

		CsgWorld csgWorld;
		GraphicWorld graphicWorld;
		TextureManager textureManager;
		FontManager fontManager;

	private:
		void addDefaultObjects();
};