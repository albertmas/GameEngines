#ifndef __ModuleImGui_H__
#define __ModuleImGui_H__

#include "Module.h"
#include "Globals.h"

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

	int number = 0;

public:
	bool testwindow = false;
	bool spherewindow = false;
	bool trianglewindow = false;
	bool randomwindow = false;


	int radius = 0;
	int x = 0, y = 0, z = 0;


	float3 pos;
	float3 min_pos;
	float3 max_pos;
};

#endif //__ModuleImGui_H__