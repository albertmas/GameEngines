#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"
#include "ModuleAudio.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");
	
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	char* title = "3D Engine";
	App->window->SetTitle(title);

	return UPDATE_CONTINUE;
}

vec3 ModulePlayer::Player_to_World(vec3 vector_to_change, bool translate)
{
	//vec3 player_pos = vehicle->GetPos();
	//mat3x3 rotation = vehicle->GetRotation();

	////We need to put it in the world frame, so we need: Origin of the player respect the world (player_pos) and Rotation matrix of the player respect the world frame(rotation).
	//vec3 ret(0, 0, 0);
	//if (translate)
	//	ret = rotation*vector_to_change + player_pos;
	//else
	//	ret = rotation*vector_to_change;

	//return ret;
	return 0;
}

vec3 ModulePlayer::World_to_Player(vec3 vector_to_change, bool translate)
{
	//vec3 player_pos = vehicle->GetPos();
	//mat3x3 rotation = vehicle->GetRotation();

	////We need to put it in the player frame, so we need: Origin of the world respect the player (-player_pos) and Rotation matrix of the world frame respect the player(rotation').
	//rotation = App->physics->translate_3x3mat(rotation);
	//player_pos = -player_pos;

	//vec3 ret(0, 0, 0);
	//if (translate)
	//	ret = rotation*vector_to_change + player_pos;
	//else
	//	ret = rotation*vector_to_change;

	//return ret;
	return 0;
}

//vec3 ModulePlayer::btVector3_to_vec3(btVector3 vector)
//{
//	return vec3(vector.getX(), vector.getY(), vector.getZ());
//}
