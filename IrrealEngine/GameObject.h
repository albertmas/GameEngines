#ifndef _GameObject_H_
#define _GameObject_H_

#include "Globals.h"
#include "Component.h"
#include "MathGeoLib/MathGeoLib.h"

class Component;
class ComponentTransform;
enum COMP_TYPE;
class Camera;
class ComponentMesh;
class GameObject
{
public:
	GameObject(GameObject* parent, const char* name);
	~GameObject();

	bool Init();
	void Update();
	bool CleanUp();

	void Draw();

	void SetName(const char* name); //???


	Component* CreateComponent(Component::COMP_TYPE type);
	Component* GetComponent(Component::COMP_TYPE type);
	void PushComponent(Component* new_component); //???

	bool IsStatic()const;
	bool HasMesh()const;
	bool IsRoot()const;

	bool HasCam()const;

	//AABB GetBB();

	Camera* GetCamera();

public:
	bool go_active = true;
	bool go_static = false;
	bool root_go = false; //???

	std::string go_name = "";
	std::vector<Component*> go_components;
	GameObject* go_parent = nullptr;
	std::vector<GameObject*> go_children;

	ComponentTransform* comp_transform;

	AABB local_AABB;
	AABB global_AABB;
	OBB oriented_BB;
};

#endif