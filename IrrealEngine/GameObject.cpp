#include "GameObject.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentTexture.h"
#include "ComponentMaterial.h"


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
		go_components.push_back(comp);
		break;
	case TRANSFORMATION:
		comp = new ComponentTransform();
		go_components.push_back(comp);
		break;
	case TEXTURE:
		comp = new ComponentMaterial();
		go_components.push_back(comp);
		break;
	case MATERIAL:
		comp = new ComponentMaterial();
		go_components.push_back(comp);
		break;

	default:
		break;
	}

	return comp;
}
