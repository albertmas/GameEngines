#ifndef __ModuleMeshLoader_H__
#define __ModuleMeshLoader_H__

#include "Module.h"


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
	FBXMesh* LoadMesh(const char* full_path);
};

#endif