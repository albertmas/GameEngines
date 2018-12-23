#include "GameObject.h"
#include "Application.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentTexture.h"
#include "ComponentCamera.h"
#include "ComponentAudioSource.h"
#include "ComponentAudioListener.h"
#include "ModuleScene.h"
#include "ModuleRenderer3D.h"
#include "ModuleImGui.h"

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

						CalcGlobalTransform();

						App->renderer3D->DrawBB(global_AABB, { 1, 0, 0 });
						App->renderer3D->DrawBB(oriented_BB, { 0, 1, 0 });
					}
				}

				ComponentAudioSource* comp_audio_source = (ComponentAudioSource*)GetComponent(Component::AUDIOSOURCE);
				if (comp_audio_source != nullptr)
					comp_audio_source->Update();

				ComponentAudioListener* comp_audio_listener = (ComponentAudioListener*)GetComponent(Component::AUDIOLISTENER);
				if (comp_audio_listener != nullptr)
					comp_audio_listener->Update();
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
	case Component::CAMERA:
		comp = new ComponentCamera(this);
		go_components.push_back(comp);
		break;
	case Component::AUDIOSOURCE:
		comp = new ComponentAudioSource(this);
		go_components.push_back(comp);
		break;
	case Component::AUDIOLISTENER:
		comp = new ComponentAudioListener(this);
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

void GameObject::CalcGlobalTransform()
{
	ComponentTransform* trans = GetComponent(Component::TRANSFORMATION)->AsTransform();
	if (go_parent != nullptr)
	{
		trans->matrix_global = go_parent->GetComponent(Component::TRANSFORMATION)->AsTransform()->matrix_global * trans->matrix_local;
	}
	OBB newobb = local_AABB;
	newobb.Transform(trans->matrix_global);
	oriented_BB = newobb;

	global_AABB.SetNegativeInfinity();
	global_AABB.Enclose(oriented_BB);

	for (std::vector<GameObject*>::iterator item = go_children.begin(); item != go_children.end(); item++)
	{
		(*item)->CalcGlobalTransform();
	}
}

void GameObject::IsPickedABB(LineSegment picking, std::vector<GameObject*> &vec)
{
	//Deselect();
	App->imgui->focused_go = nullptr;

	if (!go_children.empty())
	{
		for (std::vector<GameObject*>::iterator it = go_children.begin(); it != go_children.end(); ++it)
		{
			(*it)->IsPickedABB(picking, vec);
		}
	}

	if (global_AABB.Intersects(picking))
	{
		//selected = true;
		vec.push_back(this);
	}
	//else
	//	selected = false;
}