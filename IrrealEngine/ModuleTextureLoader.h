#ifndef __ModuleTextureLoader_H__
#define __ModuleTextureLoader_H__

#include "Module.h"


struct Texture
{
	std::string name = "";

	uint width = 0;
	uint height = 0;

	uint GL_id = 0;
};

class ModuleTextureLoader :	public Module
{
public:
	ModuleTextureLoader();
	~ModuleTextureLoader();

public:
	std::list<Texture*> textures;
};

#endif