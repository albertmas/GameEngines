#ifndef COMPONENT_TEXTURE_H_
#define COMPONENT_TEXTURE_H_

#include "Component.h"

class ComponentTexture :
	public Component
{
public:
	ComponentTexture(GameObject* gameobject);
	~ComponentTexture() override;
};

#endif