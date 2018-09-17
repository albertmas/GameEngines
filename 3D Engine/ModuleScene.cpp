#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "ModulePlayer.h"
#include "ModulePlayer.h"

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleScene::~ModuleScene()
{}

// Load assets
bool ModuleScene::Start()
{
	LOG("Loading scene assets");
	bool ret = true;

	return ret;
}

// Load assets
bool ModuleScene::CleanUp()
{
	LOG("Unloading scene");

	return true;
}

// Update
update_status ModuleScene::Update(float dt)
{
	return UPDATE_CONTINUE;
}

void ModuleScene::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	LOG("Hit!");
}