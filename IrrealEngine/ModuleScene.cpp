#include "Application.h"
#include "ModuleScene.h"
#include "ModuleCamera3D.h"
#include "ModuleImGui.h"
#include "ModuleSceneLoader.h"
#include "ModuleMeshLoader.h"
#include "ModuleTextureLoader.h"
#include "ModuleInput.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentTexture.h"

#ifdef _DEBUG
#include "mmgr/mmgr.h"
#endif

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

	App->camera->Move(vec3(0.0f, 10.0f, 0.0f));
	App->camera->LookAt(vec3(0.0f, 0.0f, 0.0f));

	return ret;
}

bool ModuleScene::Start()
{
	root = new GameObject(nullptr, "root");
	root->UUID = 0;
	game_objects.push_back(root);
	ComponentTransform* root_trans = (ComponentTransform*)root->CreateComponent(Component::TRANSFORMATION);

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
								else if (strcmp(iter_comp_data->name.GetString(), "Transform") == 0)
								{
									uint num = 0;
									ComponentTransform* comp_trans = (ComponentTransform*)comp;
									for (int i = 0; i < 4; i++)
									{
										for (int t = 0; t < 4; t++)
										{
											comp_trans->matrix_local[i][t] = iter_comp_data->value.GetArray()[num].GetFloat();
											num++;
										}
									}
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
			comp_trans->matrix_local.Decompose(comp_trans->position, comp_trans->rotation, comp_trans->scale);

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
}

GameObject* ModuleScene::CreateGameObject()
{
	GameObject* gameobject = new GameObject(root, "");
	game_objects.push_back(gameobject);

	return gameobject;
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
