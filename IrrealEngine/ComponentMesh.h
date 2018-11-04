#ifndef COMPONENT_MESH_H_
#define COMPONENT_MESH_H_

#include "Component.h"
#include "ModuleFBXLoader.h"


class Mesh;
class GameObject;

class ComponentMesh :public Component
{
public:

	ComponentMesh(GameObject* gameobject);
	~ComponentMesh() override;

	bool Update() override;

	void SetMesh(FBXMesh* mesh);
	void SetInspectorInfo() override;

private:
	FBXMesh* go_mesh = nullptr;

};

#endif