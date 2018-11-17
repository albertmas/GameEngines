#ifndef __ModuleScene_H__
#define __ModuleScene_H__

#include "Module.h"
#include <list>

class GameObject;

class ModuleScene :	public Module
{
public:
	ModuleScene(bool start_enabled = true);
	~ModuleScene() override;

	bool Init(Document& document) override;
	bool Start() override;
	update_status PreUpdate(float dt) override;
	update_status Update(float dt) override;
	update_status PostUpdate(float dt) override;
	bool CleanUp() override;

	bool Save(Document& document, FileWriteStream& fws)const override;
	bool Load(Document& document) override;

	bool SaveScene(const char* file);
	bool LoadScene(const char* file);

	void Draw();
	GameObject* CreateGameObject();
	void SetGlobalMatrix(GameObject* gameobject);

public:
	std::vector<GameObject*> game_objects;
	GameObject* root = nullptr;

};

#endif