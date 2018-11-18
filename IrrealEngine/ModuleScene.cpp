#include "Application.h"
#include "ModuleScene.h"
#include "ModuleCamera3D.h"
#include "ModuleImGui.h"
#include "ModuleSceneLoader.h"
#include "ModuleMeshLoader.h"
#include "ModuleTextureLoader.h"
#include "ModuleInput.h"
#include "ModulePick.h"
#include "ModuleImGui.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentTexture.h"
#include "ComponentCamera.h"
#include "QuadTree.h"
#include "mmgr/mmgr.h"
#include "glmath.h"


ModuleScene::ModuleScene(bool start_enabled) : Module(start_enabled)
{
}

ModuleScene::~ModuleScene()
{
}


bool ModuleScene::Init(Document& document)
{
	LOG("Loading Scene");
	LOG("Loading Plane");
	bool ret = true;


	return ret;
}

bool ModuleScene::Start()
{
	root = new GameObject(nullptr, "root");
	root->UUID = 0;
	game_objects.push_back(root);
	ComponentTransform* root_trans = (ComponentTransform*)root->CreateComponent(Component::TRANSFORMATION);

	App->camera->StartEditorCam();

	GameObject* new_cam = CreateCamera();
	game_objects.push_back(new_cam);

	if (new_cam->HasCam())
		App->camera->cams_list.push_back(new_cam);
	App->camera->SetCurrentCam(new_cam);
	App->camera->NewCamera();

	GlobalQuadTree = new Quadtree(AABB(float3(-90, -70, -90), float3(60, 50, 50)), 0);

	for (uint i = 0; i < game_objects.size(); i++)
	{
		game_objects[i]->go_static= true;
	}
	for (uint i = 0; i < game_objects.size(); i++)
	{
		if (game_objects[i]->IsActive())
			GlobalQuadTree->Insert(game_objects[i]);
	}


	App->sceneloader->ImportMesh("Assets/street/Street environment_V01.fbx");//street/Street environment_V01

	return true;
}

update_status ModuleScene::PreUpdate(float dt)
{
	SetGlobalMatrix(root);

	return UPDATE_CONTINUE;
}
update_status ModuleScene::Update(float dt)
{
	Draw();

	return UPDATE_CONTINUE;
}

update_status ModuleScene::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleScene::CleanUp()
{
	LOG("Freeing all Scene elements");

	App->imgui->focused_go = nullptr;

	std::list<FBXMesh*>::iterator iter_mesh;
	iter_mesh = App->meshloader->meshes.begin();
	while (iter_mesh != App->meshloader->meshes.end())
	{
		RELEASE((*iter_mesh));
		iter_mesh++;
	}
	App->meshloader->meshes.clear();
	
	std::vector<GameObject*>::iterator iter_go;
	iter_go = root->go_children.begin();
	while (iter_go != root->go_children.end())
	{
		RELEASE((*iter_go));
		iter_go++;
	}
	game_objects.clear();
	root->go_children.clear();
	RELEASE(root);

	return true;
}

bool ModuleScene::Save(Document& document, FileWriteStream& fws)const
{
	Document::AllocatorType& allocator = document.GetAllocator();
	document.AddMember("name", "scene", allocator);
	/*rapidjson::Value index("scene", sizeof("scene"), allocator);
	Value* _value = new Value();
	_value->AddMember(index, 3, allocator);
	Writer<FileWriteStream> writer(fws);*/

	return true;
}

bool ModuleScene::Load(Document& document)
{
	assert(document.IsObject());
	assert(document["name"].IsString());

	return true;
}

bool ModuleScene::SaveScene(const char* file)
{
	bool ret = true;

	FILE* fp = fopen(file, "wb");
	if (!fp)
	{
		LOG("Schema file %s not found", file);
		ret = false;
	}
	else
	{
		LOG("--------------- Scene Saving ---------------");
		char writeBuffer[65536];
		FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
		Document document;
		document.SetObject();

		Value myArray(kObjectType);
		Document::AllocatorType& allocator = document.GetAllocator();

		// Call Save() in all GameObjects
		for (std::vector<GameObject*>::iterator item = game_objects.begin(); item != game_objects.end(); item++)
		{
			if ((*item) != root)
			{
				(*item)->Save(allocator, &myArray);
			}
		}
		document.AddMember("Scene", myArray, allocator);
		/*StringBuffer strbuf;
		Writer<StringBuffer> writer(strbuf);*/

		PrettyWriter<FileWriteStream> writer(os);
		document.Accept(writer);
		fclose(fp);
	}

	return ret;
}

bool ModuleScene::LoadScene(const char* file)
{
	bool ret = true;

	FILE* fp = fopen(file, "rb");
	if (!fp)
	{
		LOG("Schema file %s not found", file);
		ret = false;
	}
	else
	{
		CleanScene();

		LOG("--------------- Scene Loading ---------------");
		char readBuffer[65536];
		FileReadStream is(fp, readBuffer, sizeof(readBuffer));
		Document document;
		document.ParseStream(is);
		// Call Load() in all GameObjects
		assert(!document.IsNull());
		Document::AllocatorType& allocator = document.GetAllocator();

		std::vector<GameObject*> newGOlist;

		for (Value::ConstMemberIterator iter = document["Scene"].MemberBegin(); iter != document["Scene"].MemberEnd(); ++iter)
		{
			GameObject* newGameObject = new GameObject(root, "Untitled");
			newGOlist.push_back(newGameObject);

			for (Value::ConstMemberIterator iter_child = iter->value.MemberBegin(); iter_child != iter->value.MemberEnd(); ++iter_child)
			{
				if (strcmp(iter_child->name.GetString(), "UUID") == 0)
				{
					newGameObject->UUID = iter_child->value.GetUint();
				}
				else if (strcmp(iter_child->name.GetString(), "Parent_UUID") == 0)
				{
					newGameObject->UUID_parent = iter_child->value.GetUint();
				}
				else if (strcmp(iter_child->name.GetString(), "Name") == 0)
				{
					newGameObject->go_name = iter_child->value.GetString();
				}
				else if (strcmp(iter_child->name.GetString(), "Active") == 0)
				{
					newGameObject->go_active = iter_child->value.GetBool();
				}
				else if (strcmp(iter_child->name.GetString(), "Static") == 0)
				{
					newGameObject->go_static = iter_child->value.GetBool();
				}

				if (iter_child->value.IsArray())
				{
					for (Value::ConstValueIterator iter_child_comp = iter_child->value.Begin(); iter_child_comp != iter_child->value.End(); ++iter_child_comp)
					{
						if (iter_child_comp->IsObject())
						{
							Component* comp = nullptr;

							for (Value::ConstMemberIterator iter_comp_data = iter_child_comp->MemberBegin(); iter_comp_data != iter_child_comp->MemberEnd(); ++iter_comp_data)
							{
								if (strcmp(iter_comp_data->name.GetString(), "Type") == 0)
								{
									if (iter_comp_data->value.GetInt() == Component::TRANSFORMATION)
									{
										comp = newGameObject->CreateComponent(Component::TRANSFORMATION);
									}
									else if (iter_comp_data->value.GetInt() == Component::MESH)
									{
										comp = newGameObject->CreateComponent(Component::MESH);
									}
									else if (iter_comp_data->value.GetInt() == Component::TEXTURE)
									{
										comp = newGameObject->CreateComponent(Component::TEXTURE);
									}
									comp->type = (Component::COMP_TYPE)iter_comp_data->value.GetInt();
								}
								else if (strcmp(iter_comp_data->name.GetString(), "Active") == 0)
								{
									comp->active = iter_comp_data->value.GetBool();
								}
								else if (strcmp(iter_comp_data->name.GetString(), "UUID") == 0)
								{
									comp->UUID = iter_comp_data->value.GetUint();
								}
								else if (strcmp(iter_comp_data->name.GetString(), "Position") == 0)
								{
									ComponentTransform* comp_trans = (ComponentTransform*)comp;
									comp_trans->position.x = iter_comp_data->value.GetArray()[0].GetFloat();
									comp_trans->position.y = iter_comp_data->value.GetArray()[1].GetFloat();
									comp_trans->position.z = iter_comp_data->value.GetArray()[2].GetFloat();
								}
								else if (strcmp(iter_comp_data->name.GetString(), "Rotation") == 0)
								{
									ComponentTransform* comp_trans = (ComponentTransform*)comp;
									comp_trans->rotation.x = iter_comp_data->value.GetArray()[0].GetFloat();
									comp_trans->rotation.y = iter_comp_data->value.GetArray()[1].GetFloat();
									comp_trans->rotation.z = iter_comp_data->value.GetArray()[2].GetFloat();
								}
								else if (strcmp(iter_comp_data->name.GetString(), "Scale") == 0)
								{
									ComponentTransform* comp_trans = (ComponentTransform*)comp;
									comp_trans->scale.x = iter_comp_data->value.GetArray()[0].GetFloat();
									comp_trans->scale.y = iter_comp_data->value.GetArray()[1].GetFloat();
									comp_trans->scale.z = iter_comp_data->value.GetArray()[2].GetFloat();
								}
								else if (strcmp(iter_comp_data->name.GetString(), "Mesh") == 0)
								{
									ComponentMesh* comp_mesh = (ComponentMesh*)comp;

									FBXMesh* newMesh = App->meshloader->LoadMesh(iter_comp_data->value.GetString());
									if (newMesh != nullptr)
									{
										comp_mesh->SetMesh(newMesh);
										newGameObject->local_AABB.SetNegativeInfinity();
										newGameObject->local_AABB.Enclose((float3*)newMesh->vertices, newMesh->num_vertices);
									}
								}
								else if (strcmp(iter_comp_data->name.GetString(), "Texture") == 0)
								{
									ComponentTexture* comp_tex = (ComponentTexture*)comp;

									std::string texPath = TEXTURES_DIRECTORY;
									texPath += iter_comp_data->value.GetString();
									texPath += TEXTURES_EXTENSION;

									Texture* newTex = App->texloader->LoadTexture(texPath.c_str());
									if (newTex != nullptr)
									{
										newTex->name = iter_comp_data->value.GetString();
										comp_tex->texture = newTex;
									}

								}
								else if (strcmp(iter_comp_data->name.GetString(), "Color") == 0)
								{
									ComponentTexture* comp_tex = (ComponentTexture*)comp;
									if (comp_tex->texture == nullptr)
									{
										comp_tex->texture = new Texture();
										comp_tex->texture->tex = false;
									}

									comp_tex->texture->color.x = iter_comp_data->value.GetArray()[0].GetFloat();
									comp_tex->texture->color.y = iter_comp_data->value.GetArray()[1].GetFloat();
									comp_tex->texture->color.z = iter_comp_data->value.GetArray()[2].GetFloat();
								}
							}
						}
					}
				}
			}
		}

		fclose(fp);

		// Set correct parents & transformation
		for (int t = 0; t < newGOlist.size(); t++)
		{
			ComponentTransform* comp_trans = (ComponentTransform*)newGOlist[t]->GetComponent(Component::TRANSFORMATION);
			comp_trans->matrix_local.Set(float4x4::FromTRS(comp_trans->position, comp_trans->rotation, comp_trans->scale));

			newGOlist[t]->ChangeParent(newGOlist, newGOlist[t]->UUID_parent);
		}
	}

	return ret;
}

void ModuleScene::Draw()
{
	for (int i = 0; i < game_objects.size(); i++)
	{
		if (game_objects[i] != root)
			game_objects[i]->Draw();
	}
	if (App->camera->GetCurrentCam()->frustrum_draw)
		App->camera->GetCurrentCam()->DrawFrustum();

	if (App->imgui->mouse_ray)
		App->ray->DrawRay();
}

void ModuleScene::CleanScene()
{
	std::list<FBXMesh*>::iterator iter_mesh;
	iter_mesh = App->meshloader->meshes.begin();
	while (iter_mesh != App->meshloader->meshes.end())
	{
		RELEASE((*iter_mesh));
		iter_mesh++;
	}
	App->meshloader->meshes.clear();

	CleanChildrenGO(root);

	root->go_children.clear();
	game_objects.clear();
	game_objects.push_back(root);
}

void ModuleScene::CleanChildrenGO(GameObject* child)
{
	std::vector<GameObject*>::iterator iter_go;
	iter_go = child->go_children.begin();
	while (iter_go != child->go_children.end())
	{
		for (int i = 0; i < (*iter_go)->go_components.size(); i++)
		{
			RELEASE((*iter_go)->go_components[i]);
		}
		(*iter_go)->go_components.clear();

		if ((*iter_go)->go_children.size() > 0)
		{
			for (int i = 0; i < (*iter_go)->go_children.size(); i++)
			{
				CleanChildrenGO((*iter_go)->go_children[i]);
			}
			(*iter_go)->go_children.clear();
		}
		else
			RELEASE((*iter_go));

		iter_go++;
	}
}

GameObject* ModuleScene::CreateGameObject()
{
	GameObject* gameobject = new GameObject(root, "");
	game_objects.push_back(gameobject);

	return gameobject;
}

void ModuleScene::DrawGOBoundingBoxes()
{

	for (uint i = 0; i < game_objects.size(); i++)
	{
		if (game_objects[i]->IsActive())
			game_objects[i]->RenderBoundingBox();


	}

}
void ModuleScene::SetGlobalMatrix(GameObject* gameobject)
{
	ComponentTransform* compTrans = (ComponentTransform*)gameobject->GetComponent(Component::TRANSFORMATION);

	if (compTrans != nullptr)
	{
		if (gameobject->go_parent != nullptr)
			compTrans->matrix_global = ((ComponentTransform*)gameobject->go_parent->GetComponent(Component::TRANSFORMATION))->matrix_global * compTrans->matrix_local;

		for (int i = 0; i < gameobject->go_children.size(); i++)
		{
			SetGlobalMatrix(gameobject->go_children[i]);
		}
	}
}

GameObject* ModuleScene::CreateCamera()
{
	GameObject* main_camera_go = new GameObject(root, "Main Camera");
	ComponentCamera* cam_comp = new ComponentCamera();
	main_camera_go->PushComponent(cam_comp);
	cam_comp->cam->SetFarPlane(1000);
	
	return main_camera_go;
}

GameObject * ModuleScene::Closest_go(std::vector<GameObject*> gameobjects, LineSegment line)
{
	float3 closest_point;
	float closest_distance = 99999;
	GameObject* closest_go = nullptr;
	float distance;

	for (std::vector<GameObject*>::iterator it = gameobjects.begin(); it != gameobjects.end(); it++)
	{
		GameObject* go = (*it);

		ComponentMesh* mesh = go->GetComponentMesh();

		if (go->HasMesh())
		{
			float3 point = { 0,0,0 };

			if (mesh->Firstpoint(line, point, distance))
			{
				if (distance < closest_distance)
				{
					closest_distance = distance;
					closest_point = point;
					closest_go = go;
				}
			}
		}
	}
	return closest_go;
}

void ModuleScene::ClickSelect(LineSegment mouse_ray)
{
	std::vector<GameObject*> intersected_list;

	for (std::vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); it++)
	{
		GameObject* go = (*it);

		if (!go->HasMesh())
		{
			continue;
		}

		bool collided = mouse_ray.Intersects(go->GetBB());

		if (collided)
		{
			intersected_list.push_back(go);
		}
	}


	GameObject* closestGo = Closest_go(intersected_list, mouse_ray);
	if (closestGo == nullptr)
		return;
	if (GetSelected() != nullptr)
		GetSelected()->SetSelected(false);
	closestGo->SetSelected(true);
}

GameObject* ModuleScene::GetSelected()
{
	GameObject* aux = nullptr;
	for (std::vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); it++)
	{
		if ((*it)->IsSelected() == true)
			aux = (*it);
	}

	return aux;
}

ImVec2 ModuleScene::GetPos() const
{
	return pos;
}

ImVec2 ModuleScene::GetSize() const
{
	return size;
}
