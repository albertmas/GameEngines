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

	App->camera->Move(float3(0.0f, 10.0f, 0.0f));
	App->camera->LookAt(float3(0.0f, 0.0f, 0.0f));

	return ret;
}

bool ModuleScene::Start()
{
	App->fbxloader->LoadFile("../Assets/BakerHouse.FBX");

	return true;
}

update_status ModuleScene::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}
update_status ModuleScene::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN)
		App->fbxloader->LoadFile("../Assets/BakerHouse.FBX");

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

bool ModuleScene::Draw()const
{
	return true;
}