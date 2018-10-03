#ifndef __ModuleFBXLoader_H__
#define __ModuleFBXLoader_H__

#include "Module.h"

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
};

#endif