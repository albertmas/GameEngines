#ifndef COMPONENT_MESH_H_
#define COMPONENT_MESH_H_

#include "Component.h"
#include "ModuleFBXLoader.h"


class ComponentMesh :public Component
{
public:
	ComponentMesh(GameObject* gameobject);
	~ComponentMesh() override;

	void SetMesh(FBXMesh* addMesh);

private:
	FBXMesh* mesh;
};

#endif