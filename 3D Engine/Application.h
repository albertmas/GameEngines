#pragma once


#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModulePhysics3D.h"
#include "ModuleImGui.h"

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModulePhysics3D* physics;
	ModuleImGui* imgui;
	
private:

	Timer	ms_timer;
	float	dt;
	std::list<Module*> list_modules;
	
	Timer				startup_time;
	Timer				last_sec_frame_time;

public:
	float totalVideoMemF = -1.0f;
	float currentVideoMemF = -1.0f;
	float availableVideoMemF = -1.0f;
	float reservedVideoMemF = -1.0f;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();
	void OpenWeb(const char* url);

	int					framerate_cap = 0;

	Uint64				frame_count = 0;
	Uint32				last_sec_frame_count = 0;
	Uint32				prev_last_sec_frame_count = 0;

	std::vector<float>	fps_log;
	std::vector<float>	ms_log;

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};