#ifndef _COMPONENT_TEXTURE_
#define _COMPONENT_TEXTURE_
#include "Component.h"

class Texture;

class ComponentTexture : public Component
{

public:

	ComponentTexture(GameObject* gameobject, COMP_TYPE* comp_type) :Component(gameobject) {}
	~ComponentTexture();

	bool Update();


public:

	Texture * material;
};

#endif