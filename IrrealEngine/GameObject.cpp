#include "GameObject.h"
#include "Application.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentTexture.h"
#include "ModuleScene.h"
#include "ModuleRenderer3D.h"
#include "ModuleImGui.h"
#include "ComponentCamera.h"

#include "PCG/pcg_basic.h"
#include "mmgr/mmgr.h"


GameObject::GameObject(GameObject* parent, const char* name)
{
	if (parent)
	{
		go_parent = parent;
		parent->go_children.push_back(this);
		UUID_parent = parent->UUID;
	}
	go_name = name;
	local_AABB = AABB({ 0,0,0 }, { 0,0,0 });
	UUID = pcg32_random_r(&App->rng);
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
						glBindTexture(GL_TEXTURE_2D, 0);
						glColor3f(1.0, 1.0, 1.0);

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

Value GameObject::Save(Document::AllocatorType& allocator, Value* myArray)
{
	Value GOarray(kObjectType);

	GOarray.AddMember("UUID", UUID, allocator);
	GOarray.AddMember("Parent_UUID", go_parent->UUID, allocator);
	Value name(go_name.c_str(), allocator);
	GOarray.AddMember("Name", name, allocator);
	GOarray.AddMember("Active", go_active, allocator);
	GOarray.AddMember("Static", go_static, allocator);

	Value Components(kArrayType);


	if (go_components.size() > 0)
	{
		ComponentTransform* comp_trans = (ComponentTransform*)GetComponent(Component::TRANSFORMATION);
		if (comp_trans != nullptr)
		{
			Components.PushBack(comp_trans->Save(allocator), allocator);
		}
		ComponentMesh* comp_mesh = (ComponentMesh*)GetComponent(Component::MESH);
		if (comp_mesh != nullptr)
		{
			Components.PushBack(comp_mesh->Save(allocator), allocator);
		}
		ComponentTexture* comp_tex = (ComponentTexture*)GetComponent(Component::TEXTURE);
		if (comp_tex != nullptr)
		{
			Components.PushBack(comp_tex->Save(allocator), allocator);
		}
	}
	GOarray.AddMember("Components", Components, allocator);

	std::string go_name = "GameObject";
	Value v_go_name(go_name.c_str(), allocator);
	myArray->AddMember(v_go_name, GOarray, allocator);

	if (go_children.size() > 0)
	{
		for (int i = 0; i < go_children.size(); i++)
		{
			go_children[i]->Save(allocator, myArray);
		}
	}

	return GOarray;
}

bool GameObject::Load(Document& document)
{
	return true;
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

void GameObject::ChangeParent(std::vector<GameObject*> list, uint parent_UUID)
{
	for (int i = 0; i < list.size(); i++)
	{
		if (parent_UUID == 0)
		{
			go_parent = App->scene->root;
			UUID_parent = App->scene->root->UUID;
			App->scene->game_objects.push_back(this);
			break;
		}
		else if (list[i]->UUID == parent_UUID)
		{
			if (go_parent != nullptr)
			{
				for (int t = 0; t < go_parent->go_children.size(); t++)
				{
					if (go_parent->go_children[t] == this)
						go_parent->go_children.erase(go_parent->go_children.begin() + t);
				}
			}

			go_parent = list[i];
			UUID_parent = parent_UUID;
			list[i]->go_children.push_back(this);
		}
	}
}

ComponentMesh* GameObject::GetComponentMesh()
{
	if (this->IsActive())
	{
		ComponentMesh* mesh_tmp = (ComponentMesh*)GetComponent(Component::COMP_TYPE::MESH);
		if (mesh_tmp != nullptr)
			return mesh_tmp;
		else
			return nullptr;
	}
}

void GameObject::PushComponent(Component * new_component)
{
	go_components.push_back(new_component);
}


bool GameObject::IsStatic() const
{
	return go_static;
}

bool GameObject::IsActive() const
{
	return go_active;
}

bool GameObject::IsSelected() const
{
	return go_selected;
}
void GameObject::SetSelected(bool selected)
{
	this->go_selected = selected;
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

void GameObject::RenderBoundingBox()
{
	glLineWidth(2.5f);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex3f(local_AABB.CornerPoint(0).x, local_AABB.CornerPoint(0).y, local_AABB.CornerPoint(0).z);
	glVertex3f(local_AABB.CornerPoint(1).x,local_AABB.CornerPoint(1).y, local_AABB.CornerPoint(1).z);
	glVertex3f(local_AABB.CornerPoint(0).x,local_AABB.CornerPoint(0).y, local_AABB.CornerPoint(0).z);
	glVertex3f(local_AABB.CornerPoint(2).x,local_AABB.CornerPoint(2).y, local_AABB.CornerPoint(2).z);
	glVertex3f(local_AABB.CornerPoint(0).x,local_AABB.CornerPoint(0).y, local_AABB.CornerPoint(0).z);
	glVertex3f(local_AABB.CornerPoint(4).x,local_AABB.CornerPoint(4).y, local_AABB.CornerPoint(4).z);
	glVertex3f(local_AABB.CornerPoint(3).x,local_AABB.CornerPoint(3).y, local_AABB.CornerPoint(3).z);
	glVertex3f(local_AABB.CornerPoint(1).x,local_AABB.CornerPoint(1).y, local_AABB.CornerPoint(1).z);
	glVertex3f(local_AABB.CornerPoint(3).x,local_AABB.CornerPoint(3).y, local_AABB.CornerPoint(3).z);
	glVertex3f(local_AABB.CornerPoint(2).x,local_AABB.CornerPoint(2).y, local_AABB.CornerPoint(2).z);
	glVertex3f(local_AABB.CornerPoint(3).x,local_AABB.CornerPoint(3).y, local_AABB.CornerPoint(3).z);
	glVertex3f(local_AABB.CornerPoint(7).x,local_AABB.CornerPoint(7).y, local_AABB.CornerPoint(7).z);
	glVertex3f(local_AABB.CornerPoint(6).x,local_AABB.CornerPoint(6).y, local_AABB.CornerPoint(6).z);
	glVertex3f(local_AABB.CornerPoint(2).x,local_AABB.CornerPoint(2).y, local_AABB.CornerPoint(2).z);
	glVertex3f(local_AABB.CornerPoint(6).x,local_AABB.CornerPoint(6).y, local_AABB.CornerPoint(6).z);
	glVertex3f(local_AABB.CornerPoint(4).x,local_AABB.CornerPoint(4).y, local_AABB.CornerPoint(4).z);
	glVertex3f(local_AABB.CornerPoint(6).x,local_AABB.CornerPoint(6).y, local_AABB.CornerPoint(6).z);
	glVertex3f(local_AABB.CornerPoint(7).x,local_AABB.CornerPoint(7).y, local_AABB.CornerPoint(7).z);
	glVertex3f(local_AABB.CornerPoint(5).x,local_AABB.CornerPoint(5).y, local_AABB.CornerPoint(5).z);
	glVertex3f(local_AABB.CornerPoint(1).x,local_AABB.CornerPoint(1).y, local_AABB.CornerPoint(1).z);
	glVertex3f(local_AABB.CornerPoint(5).x,local_AABB.CornerPoint(5).y, local_AABB.CornerPoint(5).z);
	glVertex3f(local_AABB.CornerPoint(4).x,local_AABB.CornerPoint(4).y, local_AABB.CornerPoint(4).z);
	glVertex3f(local_AABB.CornerPoint(5).x,local_AABB.CornerPoint(5).y, local_AABB.CornerPoint(5).z);
	glVertex3f(local_AABB.CornerPoint(7).x,local_AABB.CornerPoint(7).y, local_AABB.CornerPoint(7).z);
	glEnd();
	glColor3f(1, 1, 1);
	glLineWidth(1.0f);
}