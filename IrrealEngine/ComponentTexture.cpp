#include "ComponentTexture.h"


ComponentTexture::ComponentTexture(GameObject* gameobject)
{
	my_go = gameobject;
	type = TEXTURE;
}

ComponentTexture::~ComponentTexture()
{
}


bool ComponentTexture :: Update()
{
	return true;
}

