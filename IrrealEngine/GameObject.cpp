#include "GameObject.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentTransform.h"


GameObject::GameObject()
{
}


GameObject::~GameObject()
{
}

bool GameObject::Init()
{
	return true;
}

bool GameObject::CleanUp()
{
	return true;
}

void GameObject::Update()
{

}

Component* GameObject::CreateComponent(COMP_TYPE type)
{
	Component* comp;

	switch (type)
	{
	case MESH:
		comp = new ComponentMesh();
		break;
	case TRANSFORMATION:
		break;
	case TEXTURE:
		break;
	case MATERIAL:
		comp = new ComponentMaterial();
		break;

	default:
		break;
	}

	return comp;
}
