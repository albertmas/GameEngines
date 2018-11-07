#ifndef __ModuleFBXLoader_H__
#define __ModuleFBXLoader_H__

#include "Module.h"
#include "Mesh.h"


struct FBXMesh;
struct aiScene;
struct aiNode;

class GameObject;

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

	bool ImportMesh(const char* full_path);
	bool LoadFile(const char* full_path, const aiScene* scene, aiNode* node, GameObject* parent);
	//void ChangeTexure(const char* full_path);
	
public:
	std::list<Mesh*> object;
	uint mesh_number = 0;
	AABB* ObjectBB = nullptr;
	uint parent_number = 0;
};

void AssimpLog(const char* str, char* userData);


#endif