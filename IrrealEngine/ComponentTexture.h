#ifndef _COMPONENT_TEXTURE_
#define _COMPONENT_TEXTURE_
#include "Component.h"

class Texture;

class ComponentTexture : public Component
{

public:

	ComponentTexture(GameObject* gameobject, COMP_TYPE comp_type) :Component(gameobject, comp_type) {}
	~ComponentTexture();

	bool Update();


public:

	Texture * material;
};

#endif