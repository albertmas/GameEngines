#ifndef __ModuleSceneLoader_H__
#define __ModuleSceneLoader_H__

#include "Module.h"
#include "Mesh.h"

#define TEXTURES_DIRECTORY "Library/Scenes/"
#define TEXTURES_EXTENSION ".IrrealScene"


struct FBXMesh;
struct aiScene;
struct aiNode;

class GameObject;

class ModuleSceneLoader : public Module
{
public:
	ModuleSceneLoader(bool start_enabled = true);
	~ModuleSceneLoader();

	bool Init(Document& document);
	update_status PreUpdate(float dt)	{	return UPDATE_CONTINUE;	}
	update_status Update(float dt)		{	return UPDATE_CONTINUE;	}
	update_status PostUpdate(float dt)	{	return UPDATE_CONTINUE;	}
	bool CleanUp();

	bool ImportMesh(const char* full_path);
	GameObject* LoadFile(const char* full_path, const aiScene* scene, aiNode* node, GameObject* parent);
	//void ChangeTexure(const char* full_path);
	
public:
	std::list<Mesh*> object;
	uint mesh_number = 0;
	AABB* ObjectBB = nullptr;
	uint parent_number = 0;
};

void AssimpLog(const char* str, char* userData);


#endif