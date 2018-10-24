#ifndef COMPONENT_MESH_H_
#define COMPONENT_MESH_H_

#include "Component.h"


class ComponentMesh :public Component
{
public:
	ComponentMesh(GameObject* gameobject);
	~ComponentMesh() override;
};

#endif