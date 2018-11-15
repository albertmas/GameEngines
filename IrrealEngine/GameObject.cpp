#include "GameObject.h"
#include "Application.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentTexture.h"
#include "ModuleRenderer3D.h"
#include "ModuleImGui.h"
#include "ComponentCamera.h"

#include "mmgr/mmgr.h"


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
		

		if (go_components.size() > 0)
		{
			ComponentTransform* comp_trans = (ComponentTransform*)GetComponent(Component::TRANSFORMATION);

			if (comp_trans != nullptr)
			{
				comp_trans->Update();

				ComponentMesh* comp_mesh = (ComponentMesh*)GetComponent(Component::MESH);
				ComponentTexture* comp_tex = (ComponentTexture*)GetComponent(Component::TEXTURE);

				if (comp_tex != nullptr && comp_mesh != nullptr)
				{
					comp_tex->Update();
				}

				if (comp_mesh != nullptr)
				{
					glPushMatrix();
					glMultMatrixf((float*)comp_trans->matrix_global.Transposed().v);
					comp_mesh->Update();
					glPopMatrix();


					if (App->renderer3D->BB || this == App->imgui->focused_go)
					{
						oriented_BB.SetNegativeInfinity();
						oriented_BB = local_AABB;
						oriented_BB.Transform(comp_trans->matrix_global);

						global_AABB.SetNegativeInfinity();
						global_AABB.Enclose(oriented_BB);

						App->renderer3D->DrawBB(global_AABB, { 1, 0, 0 });
						App->renderer3D->DrawBB(oriented_BB, { 0, 1, 0 });
					}
				}
			}
		}

		glBindTexture(GL_TEXTURE_2D, 0);
		glColor3f(1.0, 1.0, 1.0);

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


void GameObject::PushComponent(Component * new_component)
{
	go_components.push_back(new_component);
}


bool GameObject::IsStatic() const
{
	return go_static;
}

bool GameObject::HasMesh() const
{
	bool ret = false;
	for (int i = 0; i < go_components.size(); i++)
	{
		if (go_components[i]->type == Component::COMP_TYPE::MESH)
			ret = true;
	}
	return ret;
}

AABB GameObject::GetBB()
{
	ComponentMesh* aux;
	if (HasMesh())
	{
		aux = (ComponentMesh*)GetComponent(Component::COMP_TYPE::MESH);
		return aux->mesh->bounding_box;
	}
	else
		LOG("Can't return BB without a mesh");
}

bool GameObject::IsRoot() const
{
	return root_go;
}

Camera * GameObject::GetCamera()
{
	ComponentCamera* aux = (ComponentCamera*)this->GetComponent(Component::COMP_TYPE::CAMERA);
	if (aux != nullptr)
		return aux->cam;

	return nullptr;
}

bool GameObject::HasCam() const
{
	bool ret = false;
	for (int i = 0; i < go_components.size(); i++)
	{
		if (go_components[i]->type == Component::COMP_TYPE::CAMERA)
			ret = true;
	}
	return ret;
}
