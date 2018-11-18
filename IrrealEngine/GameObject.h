#ifndef _GameObject_H_
#define _GameObject_H_

#include "Globals.h"
#include "Component.h"
#include "MathGeoLib/MathGeoLib.h"
#include "Mesh.h"

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

	Value Save(Document::AllocatorType& allocator, Value* myArray);
	bool Load(Document& document);

	Component* CreateComponent(Component::COMP_TYPE type);
	Component* GetComponent(Component::COMP_TYPE type);
	void ChangeParent(std::vector<GameObject*> list, uint parent_UUID);

public:
	ComponentMesh* GetComponentMesh();

	void PushComponent(Component* new_component);

	bool IsStatic()const;
	bool IsActive()const;
	bool IsSelected()const;
	bool HasMesh()const;
	bool IsRoot()const;

	bool HasCam()const;

	void SetSelected(bool selected);

	AABB GetBB();
	void RenderBoundingBox();

	Camera* GetCamera();

public:
	bool go_active = true;
	bool go_selected = false;
	bool go_static = true;
	bool root_go = false;
	std::string go_name = "";
	uint UUID = 0;
	uint UUID_parent = 0;
	std::vector<Component*> go_components;
	GameObject* go_parent = nullptr;
	std::vector<GameObject*> go_children;

	ComponentTransform* comp_transform;

	AABB local_AABB;
	AABB global_AABB;
	OBB oriented_BB;
};

#endif