#ifndef COMPONENT_MESH_H_
#define COMPONENT_MESH_H_

#include "Component.h"


class FBXMesh;
class GameObject;
class ComponentTexture;

class ComponentMesh : public Component
{
public:

	ComponentMesh(GameObject* gameobject);
	~ComponentMesh() override;

	bool Update() override;
	void SetInspectorInfo() override;
	void SetMesh(FBXMesh* mesh);
	void SetCompTexture(ComponentTexture* compTex);

	bool Save(Document& document, FileWriteStream& fws) const override;
	bool Load(Document& document) override;

private:
	FBXMesh* go_mesh = nullptr;
	ComponentTexture* mesh_material = nullptr;
};

#endif