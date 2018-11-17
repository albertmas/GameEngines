#ifndef _GameObject_H_
#define _GameObject_H_

#include "Globals.h"
#include "Component.h"
#include "MathGeoLib/MathGeoLib.h"

class Component;

class GameObject
{
public:
	GameObject(GameObject* parent, const char* name);
	~GameObject();

	bool Init();
	void Update();
	bool CleanUp();

	void Draw();

	Value Save(Document::AllocatorType& allocator, Value* myArray);
	bool Load(Document& document);

	Component* CreateComponent(Component::COMP_TYPE type);
	Component* GetComponent(Component::COMP_TYPE type);

public:
	bool go_active = true;
	bool go_static = true;

	std::string go_name = "";
	uint UUID = 0;
	std::vector<Component*> go_components;
	GameObject* go_parent = nullptr;
	std::vector<GameObject*> go_children;

	AABB local_AABB;
	AABB global_AABB;
	OBB oriented_BB;
};

#endif