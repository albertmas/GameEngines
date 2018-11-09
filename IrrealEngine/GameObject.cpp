#include "GameObject.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentTexture.h"


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
	for (int i = 0; i < go_components.size(); i++)
	{
		RELEASE(go_components[i]);
	}
	go_components.clear();

	for (int i = 0; i < go_children.size(); i++)
	{
		RELEASE(go_children[i]);
	}
	go_children.clear();
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

void GameObject::Draw()
{
	if (go_active)
	{
		for (int i = 0; i < go_components.size(); i++)
		{
			if (go_components[i]->active)
			{
				if (go_components[i]->type == Component::TRANSFORMATION)
				{
					go_components[i]->Update();
				}
				if (go_components[i]->type == Component::TEXTURE)
				{
					go_components[i]->Update();
				}
				if (go_components[i]->type == Component::MESH)
				{
					go_components[i]->Update();
				}
			}
		}

		glBindTexture(GL_TEXTURE_2D, 0);

		for (int i = 0; i < go_children.size(); i++)
		{
			go_children[i]->Draw();
		}
	}
}

//void GameObject::DrawBB(const AABB& BB, vec3 color) const
//{
//
//}

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
		comp = new ComponentTexture(this);
		go_components.push_back(comp);
		break;
	default:
		break;
	}

	return comp;
}
