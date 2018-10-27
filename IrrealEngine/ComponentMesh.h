#ifndef COMPONENT_MESH_H_
#define COMPONENT_MESH_H_
#include "Component.h"

class Mesh;
class GameObject;

class ComponentMesh :public Component
{
public:
	ComponentMesh(GameObject* parent, COMP_TYPE comp_type);
	~ComponentMesh();

	bool Update();
};

#endif