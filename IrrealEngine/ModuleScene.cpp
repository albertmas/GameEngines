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
	for (std::list<Mesh*>::iterator it = scene_objects.begin(); it != scene_objects.end(); it++)
	{
		(*it)->DrawMesh();
	}

}

//GameObject* ModuleScene::CreateGameObject()
//{
//
//}
