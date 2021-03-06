#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "Globals.h"
#include "Timer.h"
#include "ImGui/imgui.h"
#include "SDL/include/SDL.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/filewritestream.h"
#include "PCG/pcg_basic.h"


class Module;
class ModuleWindow;
class ModuleInput;
class ModuleRenderer3D;
class ModuleCamera3D;
class ModulePhysics3D;
class ModuleAudio;
class ModuleImGui;
class ModuleSceneLoader;
class ModuleMeshLoader;
class ModuleTextureLoader;
class ModuleScene;


class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModulePhysics3D* physics;
	ModuleAudio* audio;
	ModuleImGui* imgui;
	ModuleSceneLoader* sceneloader;
	ModuleMeshLoader* meshloader;
	ModuleTextureLoader* texloader;
	ModuleScene* scene;
	
	
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
	bool SaveGame();
	bool LoadGame();

	int					framerate_cap = 0;

	Uint64				frame_count = 0;
	Uint32				last_sec_frame_count = 0;
	Uint32				prev_last_sec_frame_count = 0;

	std::vector<float>	fps_log;
	std::vector<float>	ms_log;
	std::vector<float>	memory_log;

	bool wantToSave = false;
	bool wantToLoad = false;

	pcg32_random_t rng = PCG32_INITIALIZER;

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};

extern Application* App;

#endif