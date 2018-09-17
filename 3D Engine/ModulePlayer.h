#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

struct PhysVehicle3D;

#define MAX_ACCELERATION 1000.0f
#define TURN_DEGREES 15.0f * DEGTORAD
#define BRAKE_POWER 1000.0f

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();
	vec3 World_to_Player(vec3, bool translate = true); //Given a vector in the world frame, this funtion returns it in the player frame
	vec3 Player_to_World(vec3, bool translate = true); //Given a vector in the player frame, this funtion returns it in the world frame 

	//Usefulll transformation functions
	//vec3 btVector3_to_vec3(btVector3 vector); not working for unknown reason

public:

};