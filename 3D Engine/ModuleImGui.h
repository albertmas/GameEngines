#ifndef __ModuleImGui_H__
#define __ModuleImGui_H__

#include "Module.h"

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

private:
	bool testwindow = false;
	bool testwindow2 = false;
	bool randomwindow = false;
};

#endif //__ModuleImGui_H__