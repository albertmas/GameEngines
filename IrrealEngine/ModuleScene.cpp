#include "Application.h"
#include "ModuleScene.h"
#include "ModuleMeshLoader.h"
#include "ModuleCamera3D.h"
#include "ModuleSceneLoader.h"
#include "ModuleInput.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentTexture.h"

#include "mmgr/mmgr.h"


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

	App->sceneloader->ImportMesh("Assets/BakerHouse/BakerHouse.fbx");

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
				std::string go_name = "GameObject";
				Value v_go_name(go_name.c_str(), allocator);
				myArray.AddMember(v_go_name, (*item)->Save(allocator, &myArray), allocator);
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
		LOG("--------------- Scene Loading ---------------");
		char readBuffer[65536];
		FileReadStream is(fp, readBuffer, sizeof(readBuffer));
		Document document;
		document.ParseStream(is);
		// Call Load() in all GameObjects
		assert(!document.IsNull());
		for (std::vector<GameObject*>::iterator item = game_objects.begin(); item != game_objects.end(); item++)
		{
			ret = (*item)->Load(document);
		}
		fclose(fp);
	}

	return ret;
}

void ModuleScene::Draw()
{
	for (int i = 0; i < game_objects.size(); i++)
	{
		if (game_objects[i] != root)
			game_objects[i]->Draw();
		/*if (BB && game_objects.size() > 1)
		{
			DrawBB((*iter)->bounding_box, { 1.0f, 0.0f, 0.0f });
		}*/
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
