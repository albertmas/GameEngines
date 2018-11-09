#ifndef __ModuleMeshLoader_H__
#define __ModuleMeshLoader_H__

#include "Module.h"

#define MESHES_DIRECTORY "Library/Meshes/"
#define MESHES_EXTENSION ".uwu"


class FBXMesh;
class aiMesh;

class ModuleMeshLoader : public Module
{
public:
	ModuleMeshLoader(bool start_enabled = true);
	~ModuleMeshLoader();

	//bool Init(Document& document);
	//bool CleanUp();

	bool ImportMesh(aiMesh* mesh);
	bool SaveMesh(FBXMesh* mesh);
	FBXMesh* LoadMesh(const char* name);
};

#endif