#ifndef __ModuleTextureLoader_H__
#define __ModuleTextureLoader_H__

#include "Module.h"

#define TEXTURES_DIRECTORY "Library/Textures/"
#define TEXTURES_EXTENSION ".dds"


struct Texture
{
	~Texture();

	std::string name = "";
	std::string path = "";
	uint id = 0;
	uint width = 0;
	uint height = 0;
	float3 color = { 1, 1, 1 };
};

class ModuleTextureLoader :	public Module
{
public:
	ModuleTextureLoader();
	~ModuleTextureLoader();

	bool Init(Document& document);
	bool CleanUp();

	bool ImportTexture(const char* path, std::string& output_file);
	Texture* LoadTexture(const char* full_path);

public:
	std::list<Texture*> textures;
};

#endif