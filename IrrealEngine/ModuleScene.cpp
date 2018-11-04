#include "Application.h"
#include "ModuleScene.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleFBXLoader.h"
#include "ModuleInput.h"




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
	game_objects.push_back(root);

	App->fbxloader->ImportMesh("Assets/BakerHouse/BakerHouse.fbx");

	return true;
}

update_status ModuleScene::PreUpdate(float dt)
{
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
	
	return true;
}

bool ModuleScene::Save(Document& document, FileWriteStream& fws)
{
	return true;
}

bool ModuleScene::Load(Document& document)
{
	return true;
}

void ModuleScene::Draw()
{
	for (int i = 0; i < game_objects.size(); i++)
	{
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
