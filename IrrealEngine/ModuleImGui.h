#ifndef __ModuleImGui_H__
#define __ModuleImGui_H__


#include "Module.h"
#include "ImGui/imgui.h"


class GameObject;

class ModuleImGui :	public Module
{
public:
	ModuleImGui(bool start_enabled = true);
	~ModuleImGui();

	
	bool Init(Document& document);
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	bool Save(Document& document, FileWriteStream& fws);
	bool Load(Document& document);

	void ManageInput(SDL_Event* e)const;
	void GetConsoleLog(const char* log);

	// Menu functions
	void RandomGenerator();
	void TestWindow();
	void Console();
	void ConfigurationWindow();
	void InspectorWindow();
	void HierarchyWindow();
	void CreateSphere();
	void CreateTriangle();
	void GetHardWareData();

	void DrawImgui();

	void SearchFolder(const char* path);
	void SearchGO(GameObject* parent);

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
	bool cubewindow = false;
	
	bool randomwindow = false;
	bool consolewindow = true;
	bool configurationwindow = true;
	bool inspectorwindow = true;
	bool hierarchywindow = true;
	bool aboutwindow = false;
	bool editorwindow = true;

	int radius = 0;
	int x = 0, y = 0, z = 0;
	int a = 0, b = 0, c = 0;

	float brightness = 1.0f;

	float3 pos = { 0.0f, 0.0f, 0.0f };
	float3 min_pos = { 0.0f, 0.0f, 0.0f };
	float3 max_pos = { 0.0f, 0.0f, 0.0f };

	float3 pos_a = { 0.0f, 0.0f, 0.0f };
	float3 pos_b = { 0.0f, 0.0f, 0.0f };
	float3 pos_c = { 0.0f, 0.0f, 0.0f };

	ImGuiTextBuffer consolelog;

	char selected_file[50] = "";
	std::string selected_file_path = "";
	std::string selected_file_type = "";

	GameObject* focused_go = nullptr;

private:
	bool fullscreen = false;
	bool resizable = false;
	bool borderless = false;
	bool fulldesktop = false;

	bool vsync = true;
	bool loadfile = false;

	bool scrollconsole = false;

	bool frustrum = false;

	int config_width = 300;
	int inspector_width = 250;
	int inspector_height = 250;

	char app_name[50] = TITLE;
	char organization[50] = "UPC CITM";
	char icon_name[50] = "*default*";
	char icon_name_new[50] = "";
};

#endif //__ModuleImGui_H__