#ifndef COMPONENT_TEXTURE_H_
#define COMPONENT_TEXTURE_H_

#include "Component.h"


class Texture;

class ComponentTexture : public Component
{

public:

	ComponentTexture(GameObject* gameobject);
	~ComponentTexture();

	bool Update() override;
	void SetInspectorInfo() override;

	Value Save(Document::AllocatorType& allocator) const override;
	bool Load(Document& document) override;
	
public:

	Texture* texture = nullptr;

};

#endif