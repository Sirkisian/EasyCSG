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

		CsgWorld csgWorld;
		GraphicWorld graphicWorld;
		TextureManager textureManager;
		FontManager fontManager;
};