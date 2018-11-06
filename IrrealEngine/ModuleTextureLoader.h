#ifndef __ModuleTextureLoader_H__
#define __ModuleTextureLoader_H__

#include "Module.h"


struct Texture
{
	std::string name = "";
	std::string path = "";

	uint width = 0;
	uint height = 0;

	uint id = 0;
};

class ModuleTextureLoader :	public Module
{
public:
	ModuleTextureLoader();
	~ModuleTextureLoader();

	bool Init(Document& document);
	bool CleanUp();

	bool ImportTexture(const char* path, std::string& output_file);
	GLuint LoadTexture(const char* full_path, uint &width, uint &height);

public:
	std::list<Texture*> textures;
};

#endif