#ifndef __ModuleImGui_H__
#define __ModuleImGui_H__

#include "Module.h"
#include "Globals.h"
#include "PCG\pcg_basic.h"

class ModuleImGui :	public Module
{
public:
	ModuleImGui(bool start_enabled = true);
	~ModuleImGui();

	
	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void ManageInput(SDL_Event* e)const;

	// Menu functions
	void RandomGenerator();
	void TestWindow();
	void Console();
	void ConfigurationWindow();
	void CreateSphere();
	void CreateTriangle();
	void GetHardWareData();
	



public:
	int min_rand_int = 0, max_rand_int = 100;
	int rand_int = 0;
	float min_rand_float = 0.0, max_rand_float = 1.0;
	float rand_float = 0.0;

	pcg32_random_t rng = PCG32_INITIALIZER;

public:
	bool testwindow = false;
	bool spherewindow = false;
	bool trianglewindow = false;
	bool randomwindow = false;
	bool consolewindow = false;
	bool configurationwindow = false;
	bool aboutwindow = false;

	int radius = 0;
	int x = 0, y = 0, z = 0;
	int a = 0, b = 0, c = 0;

	float brightness = 1.0f;

	float3 pos;
	float3 min_pos;
	float3 max_pos;

	float3 pos_a;
	float3 pos_b;
	float3 pos_c;



	std::vector<char*> consolelog;
	// char consolelog[4096];

private:
	bool fullscreen = false;
	bool resizable = false;
	bool borderless = false;
	bool fulldesktop = false;

	bool active = true;
	bool loadfile = false;

	char app_name[50] = "3D Engine";
	char organization[50] = "UPC CITM";
	char icon_name[50] = "*default*";
	char icon_name_new[50] = "";
};

#endif //__ModuleImGui_H__