#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModulePhysics3D.h"
#include "ModuleImGui.h"
#include "ModuleFBXLoader.h"
#include "ModuleScene.h"
#include "Panel_Configuration.h"
#include "ImGui/imgui.h"


Application::Application()
{
	window = new ModuleWindow();
	input = new ModuleInput();
	renderer3D = new ModuleRenderer3D();
	camera = new ModuleCamera3D();
	physics = new ModulePhysics3D();
	imgui = new ModuleImGui();
	fbxloader = new ModuleFBXLoader();
	scene = new ModuleScene(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(physics);
	AddModule(fbxloader);
	
	// Scenes
	AddModule(scene);

	// GUI
	AddModule(imgui);

	// Renderer last!
	AddModule(renderer3D);
}

Application::~Application()
{
	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end(); item++)
	{
		delete (*item);
	}
}

bool Application::Init()
{
	bool ret = true;

	FILE* fp = fopen("config.json", "rb");
	if (!fp)
		printf("Schema file 'config.json' not found\n");
	char readBuffer[65536];
	FileReadStream is(fp, readBuffer, sizeof(readBuffer));
	Document document;
	document.ParseStream(is);

	// Call Init() in all modules
	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end(); item++)
	{
		ret = (*item)->Init(document);
	}

	// After all Init calls we call Start() in all modules
	LOG("--------------- Application Start ---------------");
	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end(); item++)
	{
		ret = (*item)->Start();
	}
	
	fclose(fp);
	ms_timer.Start();
	startup_time.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	frame_count++;
	last_sec_frame_count++;

	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();

	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end(); item++)
	{
		if (ret == UPDATE_CONTINUE)
			ret = (*item)->PreUpdate(dt);
	}

	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end(); item++)
	{
		if (ret == UPDATE_CONTINUE)
			ret = (*item)->Update(dt);
	}

	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end(); item++)
	{
		if (ret == UPDATE_CONTINUE)
			ret = (*item)->PostUpdate(dt);
	}

	if (last_sec_frame_time.Read() >= 1000)
	{
		last_sec_frame_time.Start();
		prev_last_sec_frame_count = last_sec_frame_count;
		last_sec_frame_count = 0;
	}

	float avg_fps = (float)frame_count / startup_time.ReadSec();
	float seconds_since_startup = startup_time.ReadSec();
	Uint32 last_frame_ms = ms_timer.Read();
	Uint32 frames_on_last_update = prev_last_sec_frame_count;

	// FPS Cap
	if (framerate_cap > 0)
	{
		Uint32 frame_delay = 1000 / framerate_cap;
		if (frame_delay > last_frame_ms)
			SDL_Delay(frame_delay - last_frame_ms);
	}

	if (ms_log.size() >= 100)
	{
		for (int a = 0; a <= ms_log.size() - 2; a++)
		{
			ms_log[a] = ms_log[a + 1];
		}
		ms_log.pop_back();
	}
	if (fps_log.size() >= 100)
	{
		for (int a = 0; a <= fps_log.size() - 2; a++)
		{
			fps_log[a] = fps_log[a + 1];
		}
		fps_log.pop_back();
	}
	if (memory_log.size() >= 100)
	{
		for (int a = 0; a <= memory_log.size() - 2; a++)
		{
			memory_log[a] = memory_log[a + 1];
		}
		memory_log.pop_back();
	}
	// ms / frame log
	last_frame_ms = ms_timer.Read();
	ms_log.push_back((float)last_frame_ms);
	// fps log
	if (last_frame_ms != 0)
		fps_log.push_back((float)(1000 / last_frame_ms));
	// used memory log
	MEMORYSTATUSEX memInfo;
	memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&memInfo);
	DWORDLONG totalVirtualMem = memInfo.ullTotalPageFile;
	DWORDLONG virtualMemUsed = memInfo.ullTotalPageFile - memInfo.ullAvailPageFile;
	PROCESS_MEMORY_COUNTERS_EX pmc;
	GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
	SIZE_T virtualMemUsedByMe = pmc.PrivateUsage;
	memory_log.push_back((float)virtualMemUsedByMe);


	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	for (std::list<Module*>::reverse_iterator item = list_modules.rbegin(); item != list_modules.rend(); item++)
	{
		ret = (*item)->CleanUp();
	}

	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}

void Application::OpenWeb(const char * url)
{
	ShellExecute(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
}

Module * Application::GetModule(int index)
{
	if (index <= list_modules.size())
	{
		int i = 0;

		for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end(); item++)
		{
			if (i == index)
				return (*item);

			i++;
		}
	}
	return nullptr;
}

bool Application::SaveGame()
{
	bool ret = true;

	FILE* fp = fopen("save.json", "wb");
	if (!fp)
	{
		LOG("Schema file 'save.json' not found");
		ret = false;
	}
	else
	{
		LOG("--------------- Game Saving ---------------");
		char writeBuffer[65536];
		FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
		Document document;
		document.SetObject();
		// Call Save() in all modules
		for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end(); item++)
		{
			ret = (*item)->Save(document, os);
		}
		Writer<FileWriteStream> writer(os);
		document.Accept(writer);
		fclose(fp);
	}

	return ret;
}

bool Application::LoadGame()
{
	bool ret = true;

	FILE* fp = fopen("save.json", "rb");
	if (!fp)
	{
		LOG("Schema file 'save.json' not found");
		ret = false;
	}
	else
	{
		LOG("--------------- Game Loading ---------------");
		char readBuffer[65536];
		FileReadStream is(fp, readBuffer, sizeof(readBuffer));
		Document document;
		document.ParseStream(is);
		// Call Load() in all modules
		assert(!document.IsNull());
		for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end(); item++)
		{
			ret = (*item)->Load(document);
		}
		fclose(fp);
	}

	return ret;
}