#include "GameObject.h"
#include "Application.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentTexture.h"
#include "ModuleRenderer3D.h"


GameObject::GameObject(GameObject* parent, const char* name)
{
	if (parent)
	{
		go_parent = parent;
		parent->go_children.push_back(this);
	}
	go_name = name;
	local_AABB = AABB({ 0,0,0 }, { 0,0,0 });
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
		ComponentTransform* c_trans = nullptr;

		for (int i = 0; i < go_components.size(); i++)
		{
			if (go_components[i]->active)
			{
				if (go_components[i]->type == Component::TRANSFORMATION)
				{
					c_trans = (ComponentTransform*)go_components[i];
					go_components[i]->Update();
				}
				if (go_components[i]->type == Component::MESH)
				{
					glPushMatrix();
					glMultMatrixf((float*)c_trans->matrix_global.Transposed().v);
					go_components[i]->Update();
					glPopMatrix();


					if (App->renderer3D->BB)
					{
						AABB newAABB = local_AABB;
						global_AABB.SetNegativeInfinity();
						global_AABB.Enclose(newAABB.Transform(c_trans->matrix_global));

						oriented_BB.SetNegativeInfinity();
						oriented_BB = local_AABB;
						oriented_BB.Transform(c_trans->matrix_global);

						App->renderer3D->DrawBB(global_AABB, { 1, 0, 0 });
						App->renderer3D->DrawBB(oriented_BB, { 0, 1, 0 });
					}
				}
				if (go_components[i]->type == Component::TEXTURE)
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

Component* GameObject::GetComponent(Component::COMP_TYPE type)
{
	Component* comp = nullptr;

	for (std::vector<Component*>::iterator it_c = go_components.begin(); it_c != go_components.end(); it_c++)
	{
		if ((*it_c)->type == type)
		{
			comp = *it_c;
			break;
		}
	}

	return comp;
}