#ifndef __ModuleImGui_H__
#define __ModuleImGui_H__

#include "Module.h"
#include "Globals.h"
#include "PCG\pcg_basic.h"

class ModuleImGui :	public Module
{
public:
	ModuleImGui(Application* app, bool start_enabled = true);
	~ModuleImGui();

	
	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

public:
	int min_rand_int = 0, max_rand_int = 100;
	int rand_int = 0;
	float min_rand_float = 0.0, max_rand_float = 1.0;
	float rand_float = 0.0;

	pcg32_random_t rng = PCG32_INITIALIZER;

public:
	bool testwindow = false;
	bool spherewindow = false;
	bool randomwindow = false;
	bool consolewindow = false;
	bool configurationwindow = false;
	bool aboutwindow = false;


	int radius = 0;
	int x = 0, y = 0, z = 0;


	float3 pos;
	float3 min_pos;
	float3 max_pos;
};

#endif //__ModuleImGui_H__