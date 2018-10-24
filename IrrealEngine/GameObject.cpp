#include "GameObject.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentTexture.h"
#include "ComponentMaterial.h"


GameObject::GameObject(GameObject* parent, const char* name)
{
	if (parent)
	{
		go_parent = parent;
		parent->go_children.push_back(this);
	}
	go_name = name;
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

Component* GameObject::CreateComponent(Component::COMP_TYPE type)
{
	Component* comp = nullptr;

	switch (type)
	{
	case Component::MESH:
		comp = new ComponentMesh(this);
		go_components.push_back(comp);
		break;
	case Component::TRANSFORMATION:
		comp = new ComponentTransform(this);
		go_components.push_back(comp);
		break;
	case Component::TEXTURE:
		comp = new ComponentMaterial(this);
		go_components.push_back(comp);
		break;
	case Component::MATERIAL:
		comp = new ComponentMaterial(this);
		go_components.push_back(comp);
		break;
	default:
		break;
	}

	return comp;
}
