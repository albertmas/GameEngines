#ifndef __ModuleFBXLoader_H__
#define __ModuleFBXLoader_H__

#include "Module.h"
#include "Open_GL.h"
#include "Mesh.h"
#include <list>


struct FBXMesh;

class ModuleFBXLoader :	public Module
{
public:
	ModuleFBXLoader(bool start_enabled = true);
	~ModuleFBXLoader();

	bool Init(Document& document);
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	bool LoadFile(const char* full_path);
	GLuint loadTexture(const char* full_path);

	//Saved used texture info
	std::string texPath = "";
	uint usedTexture = 0;
	uint usedTextureWidth = 0;
	uint usedTextureHeight = 0;

public:
	std::list<Mesh*> object;

};

#endif