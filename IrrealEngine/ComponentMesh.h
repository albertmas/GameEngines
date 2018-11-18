#ifndef COMPONENT_MESH_H_
#define COMPONENT_MESH_H_

#include "Component.h"
#include "Mesh.h"

class FBXMesh;
class GameObject;
class ComponentTexture;
class Mesh;

class ComponentMesh : public Component
{
public:

	ComponentMesh(GameObject* gameobject);
	~ComponentMesh() override;

	bool Update() override;
	void SetInspectorInfo() override;
	void SetMesh(FBXMesh* mesh);
	void SetCompTexture(ComponentTexture* compTex);

	Value Save(Document::AllocatorType& allocator) const override;
	bool Load(Document& document) override;

	bool Firstpoint(LineSegment mouse_ray, float3& firstpoint, float& point_distance);

	Mesh* mesh;

private:
	FBXMesh* go_mesh = nullptr;
	ComponentTexture* mesh_material = nullptr;
};

#endif