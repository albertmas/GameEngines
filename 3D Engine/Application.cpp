#include "Application.h"

Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	audio = new ModuleAudio(this, true);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	physics = new ModulePhysics3D(this);
	imgui = new ModuleImGui(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(audio);
	AddModule(physics);
	AddModule(imgui);
	
	// Scenes


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

	// Call Init() in all modules
	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end(); item++)
	{
		ret = (*item)->Init();
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end(); item++)
	{
		ret = (*item)->Start();
	}
	
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

	last_frame_ms = ms_timer.Read();
	ms_log.push_back((float)last_frame_ms);
	if (last_frame_ms != 0)
		fps_log.push_back((float)(1000 / last_frame_ms));

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