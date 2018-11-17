#include "Application.h"
#include "ModuleImGui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl2.h"
#include "DeviceId/DeviceId.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
#include "ModulePhysics3D.h"
#include "ModuleSceneLoader.h"
#include "ModuleMeshLoader.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "Assimp/include/version.h"
#include "DevIL/include/il.h"

#include "mmgr/mmgr.h"


ModuleImGui::ModuleImGui(bool start_enabled) : Module(start_enabled)
{
}

ModuleImGui::~ModuleImGui()
{
}


bool ModuleImGui::Init(Document& document)
{
	LOG("Loading ImGui");
	bool ret = true;

	ImGui::CreateContext();

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL2_Init();
	
	return ret;
}

bool ModuleImGui::CleanUp()
{
	LOG("Freeing all ImGui elements");
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();

	return true;
}

update_status ModuleImGui::PreUpdate(float dt)
{
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	return UPDATE_CONTINUE;
}

update_status ModuleImGui::Update(float dt)
{
	// Menu window
	if (ImGui::BeginMainMenuBar())
	{

		if (ImGui::BeginMenu("Menu"))
		{


			if (ImGui::MenuItem("Show Window")) 
			{
				testwindow = !testwindow;
			}
			
			if (ImGui::MenuItem("Quit", "ESC")) 
			{
				return UPDATE_STOP;
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Console", "Shift + 1"))
			{
				consolewindow = !consolewindow;
			}
			if (ImGui::MenuItem("Configuration", "Shift + 2"))
			{
				configurationwindow = !configurationwindow;
			}
			if (ImGui::MenuItem("Insspector", "Shift + 3"))
			{
				inspectorwindow = !inspectorwindow;
			}
			if (ImGui::MenuItem("Hierarchy", "Shift + 4"))
			{
				hierarchywindow = !hierarchywindow;
			}
			/*if (ImGui::MenuItem("Editor", "Shift + 5"))
			{
				editorwindow = !editorwindow;
			}*/

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Functions"))
		{

			if (ImGui::MenuItem("Random Number"))
			{
				randomwindow = !randomwindow;

			}
			if (ImGui::MenuItem("Create Sphere"))
			{
				spherewindow = !spherewindow;

			}
			if (ImGui::MenuItem("Create Triangle"))
			{
				trianglewindow = !trianglewindow;
			}

			if (ImGui::MenuItem("Create Cube"))
			{
				cubewindow = !cubewindow;
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("About"))
		{
			ImGui::Text("Irreal Engine");
			ImGui::Text("3D Engine in development");
			ImGui::NewLine();
			ImGui::Text("Created by:");
			if (ImGui::Selectable("Guillem Arman", false, 0, { 90, 13 }))
				App->OpenWeb("https://github.com/GuillemArman");
			ImGui::SameLine(110);
			ImGui::Text("&");
			ImGui::SameLine(130);
			if (ImGui::Selectable("Albert Mas", false, 0, { 70, 13 }))
				App->OpenWeb("https://github.com/albertmas");
			ImGui::NewLine();
			
			if (ImGui::BeginMenu("Libraries"))
			{
				if (ImGui::MenuItem("DeviceId - Version December 2015"))
				{
					App->OpenWeb("https://github.com/MatthewKing/DeviceId");
				}
				std::string imgui_version = "ImGui - Version ";
				imgui_version += ImGui::GetVersion();
				if (ImGui::MenuItem(imgui_version.c_str()))
				{
					App->OpenWeb("https://github.com/ocornut/imgui");
				}
				if (ImGui::MenuItem("MathGeoLib - Version 2.0"))
				{
					App->OpenWeb("https://github.com/juj/MathGeoLib");
				}
				std::string sdl_version = "SDL - Version ";
				SDL_version version;
				SDL_GetVersion(&version);
				sdl_version += std::to_string(version.major) + '.' + std::to_string(version.minor) + '.' + std::to_string(version.patch);
				if (ImGui::MenuItem(sdl_version.c_str()))
				{
					App->OpenWeb("https://www.libsdl.org/");
				}
				if (ImGui::MenuItem("PCG - Version 0.9 (Minimal C Implementation)"))
				{
					App->OpenWeb("http://www.pcg-random.org/");
				}
				std::string opengl_version = "OpenGL - Version ";
				opengl_version += (const char*)glGetString(GL_VERSION);
				if (ImGui::MenuItem(opengl_version.c_str()))
				{
					App->OpenWeb("https://www.opengl.org/");
				}
				std::string rapidjson_version = "RapidJSON - Version ";
				rapidjson_version += RAPIDJSON_VERSION_STRING;
				if (ImGui::MenuItem(rapidjson_version.c_str()))
				{
					App->OpenWeb("http://rapidjson.org/index.html");
				}
				std::string glew_version = "Glew - Version ";
				glew_version += (const char*)glewGetString(GLEW_VERSION);
				if (ImGui::MenuItem(glew_version.c_str()))
				{
					App->OpenWeb("https://github.com/nigels-com/glew");
				}
				std::string assimp_version = "Assimp - Version ";
				assimp_version += std::to_string(aiGetVersionMajor()) + '.' + std::to_string(aiGetVersionMinor()) + '.' + std::to_string(aiGetVersionRevision());
				if (ImGui::MenuItem(assimp_version.c_str()))
				{
					App->OpenWeb("http://www.assimp.org/");
				}
				std::string devil_version = "DevIL - Version ";
				devil_version += std::to_string(IL_VERSION);
				if (ImGui::MenuItem(devil_version.c_str()))
				{
					App->OpenWeb("http://openil.sourceforge.net/");
				}
				ImGui::EndMenu();
			}
			ImGui::NewLine();
			if (ImGui::MenuItem("-> GitHub Repository <-"))
			{
				App->OpenWeb("https://github.com/albertmas/GameEngines");
			}
			ImGui::NewLine();
			if (ImGui::MenuItem("-> MIT License <-"))
			{
				App->OpenWeb("https://github.com/albertmas/GameEngines/blob/master2/LICENSE");
			}

			ImGui::EndMenu();
		}
	}

	ImGui::EndMainMenuBar();

	if (randomwindow)RandomGenerator();
	if (testwindow)TestWindow();
	if (consolewindow)Console();
	if (configurationwindow)ConfigurationWindow();
	if (inspectorwindow)InspectorWindow();
	if (hierarchywindow)HierarchyWindow();
	if (spherewindow)CreateSphere();
	if (trianglewindow)CreateTriangle();
	if (cubewindow)App->renderer3D->CreateCube();
	
	
	// Hotkeys
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN && App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		consolewindow = !consolewindow;
	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN && App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		configurationwindow = !configurationwindow;
	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN && App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		inspectorwindow = !inspectorwindow;
	if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN && App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		hierarchywindow = !hierarchywindow;
	/*if (App->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN && App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		editorwindow = !editorwindow;*/

	if (aboutwindow)
	{
		ImGui::MenuItem("Names");
	}

	return UPDATE_CONTINUE;
}

void ModuleImGui::ManageInput(SDL_Event * e) const
{
	ImGui_ImplSDL2_ProcessEvent(e);
}

bool ModuleImGui::Save(Document& document, FileWriteStream& fws)const
{
	return true;
}

bool ModuleImGui::Load(Document& document)
{
	return true;
}

void ModuleImGui::RandomGenerator()
{
	
		ImGui::SetNextWindowSize(ImVec2(400, 220), ImGuiSetCond_Once);
		ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiSetCond_Once);
		if (ImGui::Begin("Random Number Generator", &randomwindow, ImGuiWindowFlags_MenuBar))
		{
			ImGui::Text("Random Integer");

			// Rand int
			ImGui::InputInt("Max Number", &max_rand_int);
			if (max_rand_int <= min_rand_int)
			{
				max_rand_int = min_rand_int + 1;
			}
			ImGui::InputInt("Min Number", &min_rand_int);
			if (min_rand_int >= max_rand_int)
			{
				min_rand_int = max_rand_int - 1;
			}
			ImGui::Spacing();
			if (ImGui::SmallButton("Generate Random Integer"))
			{
				rand_int = ("%i", (int)pcg32_boundedrand_r(&App->rng, (max_rand_int - min_rand_int + 1)) + min_rand_int);
			}
			ImGui::SameLine();
			if (ImGui::SmallButton("Reset"))
			{
				min_rand_int = 0;
				max_rand_int = 100;
			}
			ImGui::TextColored({ 255, 0, 0, 1 }, "%i", rand_int);
			ImGui::Spacing();

			// Rand float
			ImGui::Text("Random Float");

			ImGui::Spacing();
			if (ImGui::SmallButton("Generate Random Float"))
			{
				rand_float = ldexp(pcg32_random_r(&App->rng), -32);
			}
			ImGui::TextColored({ 255, 0, 0, 1 }, "%f", rand_float);

			ImGui::End();
		}
	

}

void ModuleImGui::TestWindow()
{
		ImGui::ShowTestWindow();
}

void ModuleImGui::Console()
{
		ImGui::SetNextWindowSize(ImVec2(App->window->width - config_width - inspector_width - 4, 200), ImGuiSetCond_Always);
		ImGui::SetNextWindowPos(ImVec2(302, App->window->height - 201), ImGuiSetCond_Always);
		ImGui::Begin("Console", &consolewindow, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_HorizontalScrollbar);
		ImGui::TextUnformatted(consolelog.begin());
		if (scrollconsole)
		{
			ImGui::SetScrollHere(1.0f);
			scrollconsole = false;
		}

		ImGui::End();
}

void ModuleImGui::ConfigurationWindow()
{
		ImGui::SetNextWindowSize(ImVec2(300, App->window->height - 22), ImGuiSetCond_Always);
		ImGui::SetNextWindowPos(ImVec2(1, 21), ImGuiSetCond_Always);
		ImGui::Begin("Configuration", &configurationwindow, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoFocusOnAppearing);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Options"))
			{
				if (ImGui::MenuItem("Load Defaults"))
				{
					// Load defs
				}
				if (ImGui::MenuItem("Load"))
				{
					// Load
					App->LoadGame();
				}
				if (ImGui::MenuItem("Save"))
				{
					// Save
					App->SaveGame();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		if (ImGui::CollapsingHeader("Application"))
		{
			ImGui::InputText("App Name", app_name, IM_ARRAYSIZE(app_name));
			ImGui::InputText("Organization", organization, IM_ARRAYSIZE(organization));
			ImGui::SliderInt("Max FPS", &App->framerate_cap, 0, 125);
			if (ImGui::Checkbox("VSync", &vsync))
			{
				if (vsync)
				{
					SDL_GL_SetSwapInterval(1);
					LOG("VSync ON ------");
				}
				else
				{
					SDL_GL_SetSwapInterval(0);
					LOG("VSync OFF ------")
				}
			}
			ImGui::Text("Limit Framerate: ");
			ImGui::SameLine();
			ImGui::TextColored({ 255, 255, 0, 1 }, "%i", App->framerate_cap);

			// Graphs
			char title[25];
			sprintf_s(title, 25, "Framerate %.1f", App->fps_log[App->fps_log.size() - 1]);
			ImGui::PlotHistogram("##framerate", &App->fps_log[0], App->fps_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
			sprintf_s(title, 25, "Milliseconds %0.1f", App->ms_log[App->ms_log.size() - 1]);
			ImGui::PlotHistogram("##milliseconds", &App->ms_log[0], App->ms_log.size(), 0, title, 0.0f, 60.0f, ImVec2(310, 100));
			sprintf_s(title, 25, "Memory Consumption");
			ImGui::PlotHistogram("##memory", &App->memory_log[0], App->memory_log.size(), 0, title, 0.0f, 100000000.0f, ImVec2(310, 100));
		}
		if (ImGui::CollapsingHeader("Window"))
		{
			// Brightness
			if (ImGui::SliderFloat("Brightness", &brightness, 0.0f, 1.0f))
				SDL_SetWindowBrightness(App->window->window, brightness);

			// Window size
			if (ImGui::SliderInt("Width", &App->window->width, 640, 1920))
				App->window->SetWinWidth(App->window->width);
			if (ImGui::SliderInt("Height", &App->window->height, 480, 1080))
				App->window->SetWinHeight(App->window->height);
			
			// Window type
			if (ImGui::Checkbox("Fullscreen", &fullscreen))
				App->window->SetFullscreen(fullscreen);
			ImGui::SameLine();
			if (ImGui::Checkbox("Full Desktop", &fulldesktop))
				App->window->SetFullDesktop(fulldesktop);
			if (ImGui::Checkbox("Borderless", &borderless))
				App->window->SetBorderless(borderless);
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Useless on Fullscreen");
			ImGui::SameLine();
			if (ImGui::Checkbox("Resizable", &resizable))
				App->window->SetResizable(resizable);
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Useless on Fullscreen");
		}
		
		if (ImGui::CollapsingHeader("Input"))
		{
			ImGui::Text("Mouse Position"); ImGui::SameLine();
			ImGui::TextColored(ImVec4(0, 1, 0, 1), "%d, %d", App->input->mouse_x, App->input->mouse_y);
			ImGui::Text("Mouse Motion:");  ImGui::SameLine();
			ImGui::TextColored(ImVec4(0, 1, 0, 1), "%d, %d", App->input->mouse_x_motion, App->input->mouse_y_motion);
			ImGui::Text("Mouse Wheel:"); ImGui::SameLine();
			ImGui::TextColored(ImVec4(0, 1, 0, 1), "%d", App->input->mouse_z);

		}
		if (ImGui::CollapsingHeader("Hardware"))
		{
			GetHardWareData();

			SDL_version version;
			SDL_GetVersion(&version);

			ImVec4 green(0, 255, 0, 255);
			ImGui::Text("SDL Version"); ImGui::SameLine();
			ImGui::TextColored(green, "%d.%d.%d", version.major, version.minor, version.patch);
			ImGui::Separator();
			ImGui::Text("CPUs:");
			ImGui::SameLine();
			ImGui::TextColored(green, "%d (Cache: %dKb)", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize());
			ImGui::Text("System RAM:");
			ImGui::SameLine();
			ImGui::TextColored(green, "%0.1fGb", ((float)SDL_GetSystemRAM() / 1024));

			ImGui::Text("Caps: "); ImGui::SameLine();

			if (SDL_Has3DNow()) ImGui::TextColored(green, "3DNow,"); ImGui::SameLine();
			if (SDL_HasAVX()) ImGui::TextColored(green, "AVX,"); ImGui::SameLine();
			if (SDL_HasAltiVec()) ImGui::TextColored(green, "AltiVec,"); ImGui::SameLine();
			if (SDL_HasMMX()) ImGui::TextColored(green, "MMX,"); ImGui::SameLine();
			if (SDL_HasRDTSC()) ImGui::TextColored(green, "RDTSC,"); ImGui::SameLine();
			if (SDL_HasSSE()) ImGui::TextColored(green, "SSE,");
			if (SDL_HasSSE2()) ImGui::TextColored(green, "SSE2,"); ImGui::SameLine();
			if (SDL_HasSSE3()) ImGui::TextColored(green, "SSE3,"); ImGui::SameLine();
			if (SDL_HasSSE41()) ImGui::TextColored(green, "SSE41,"); ImGui::SameLine();
			if (SDL_HasSSE42()) ImGui::TextColored(green, "SSE42,");

			ImGui::Separator();

			ImGui::Text("GPU: "); ImGui::SameLine();
			ImGui::TextColored(green, " vendor %s", App->renderer3D->GetGraphicsVendor());

			ImGui::Text("Brand: "); ImGui::SameLine();
			ImGui::TextColored(green, "%s", App->renderer3D->GetGraphicsModel());

			ImGui::Text("VRAM Budget: "); ImGui::SameLine();
			ImGui::TextColored(green, "%.2f Mb", App->totalVideoMemF);

			ImGui::Text("VRAM Usage: "); ImGui::SameLine();
			ImGui::TextColored(green, "%.2f Mb", App->currentVideoMemF);

			ImGui::Text("VRAM Avaliable: "); ImGui::SameLine();
			ImGui::TextColored(green, "%.2f Mb", App->availableVideoMemF);

			ImGui::Text("VRAM Reserved: "); ImGui::SameLine();
			ImGui::TextColored(green, "%.2f Mb", App->reservedVideoMemF);


		}
		if (ImGui::CollapsingHeader("3D Renderer"))
		{
			App->renderer3D->FunctionsRender();
		}
		if (ImGui::CollapsingHeader("Import Files"))
		{
			ImGui::Text("Explore:  ");
			ImGui::SameLine();
			if (ImGui::Button("Assets"))
			{
				loadfile = !loadfile;
			}
			if (loadfile)
			{
				ImGui::SetNextWindowSize(ImVec2(380, 350), ImGuiSetCond_Once);
				ImGui::SetNextWindowPos(ImVec2(400, 200), ImGuiSetCond_Once);
				if (ImGui::Begin("Load File", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
				{
					ImGui::PushID(123);
					ImGui::BeginChild(123, ImVec2(365, 290), true);

					// Search executing path
					char path_[256];
					GetCurrentDirectory(256, path_);
					// Add the assets folder to the path
					std::string path_string = path_;
					path_string += "\\Assets/*";
					SearchFolder(path_string.c_str());

					
					ImGui::EndChild();
					ImGui::PopID();

					ImGui::InputText("", selected_file, IM_ARRAYSIZE(selected_file), ImGuiInputTextFlags_ReadOnly);
					ImGui::SameLine();
					if (ImGui::Button("OK", ImVec2(50.0, 0.0)))
					{
						if (selected_file != "" && selected_file_type != "")
						{
							if (selected_file_type == "fbx" | selected_file_type == "FBX")
							{
								App->sceneloader->ImportMesh(selected_file_path.c_str());
							}
							else if (selected_file_type == "dds" | selected_file_type == "png" | selected_file_type == "jpg")
							{
								//App->fbxloader->ChangeTexure(selected_file_path.c_str());
							}
						}
						
						loadfile = false;
					}
					ImGui::SameLine();
					if (ImGui::Button("Cancel", ImVec2(50.0, 0.0)))
					{
						loadfile = false;
					}

					ImGui::End();
				}
			}
		}

		ImGui::End();
}

void ModuleImGui::InspectorWindow()
{
	if (hierarchywindow)
	{
		ImGui::SetNextWindowSize(ImVec2(250, (App->window->height - 22) / 2), ImGuiSetCond_Always);
		ImGui::SetNextWindowPos(ImVec2(App->window->width - 251, 21), ImGuiSetCond_Always);
	}
	else
	{
		ImGui::SetNextWindowSize(ImVec2(250, App->window->height - 22), ImGuiSetCond_Always);
		ImGui::SetNextWindowPos(ImVec2(App->window->width - 251, 21), ImGuiSetCond_Always);
	}
	ImGui::Begin("Inspector", &inspectorwindow, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_HorizontalScrollbar);

	if (focused_go != nullptr)
	{
		// GameObject name
		char name[60];
		strcpy_s(name, 60, focused_go->go_name.c_str());
		if (ImGui::InputText("", name, 60, ImGuiInputTextFlags_EnterReturnsTrue))
			focused_go->go_name = name;

		// GameObject active
		ImGui::Checkbox("Active", &focused_go->go_active);
		ImGui::SameLine();
		if (focused_go->go_parent->go_static)
			ImGui::Checkbox("Static", &focused_go->go_static);
		else
		{
			ImGui::PushStyleColor(ImGuiCol_Text, { 0.5f, 0.5f, 0.5f, 1.0f });
			if (ImGui::Checkbox("Static", &focused_go->go_static))
				focused_go->go_static = false;
			ImGui::PopStyleColor();
		}

		// Set components information
		for (int i = 0; i < focused_go->go_components.size(); i++)
		{
			focused_go->go_components[i]->SetInspectorInfo();
		}
	}

	ImGui::End();
}

void ModuleImGui::HierarchyWindow()
{
	if (inspectorwindow)
	{
		ImGui::SetNextWindowSize(ImVec2(250, (App->window->height - 23) / 2), ImGuiSetCond_Always);
		ImGui::SetNextWindowPos(ImVec2(App->window->width - 251, 22 + (App->window->height - 22) / 2), ImGuiSetCond_Always);
	}
	else
	{
		ImGui::SetNextWindowSize(ImVec2(250, App->window->height - 22), ImGuiSetCond_Always);
		ImGui::SetNextWindowPos(ImVec2(App->window->width - 251, 21), ImGuiSetCond_Always);
	}
	ImGui::Begin("Hierarchy", &hierarchywindow, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_HorizontalScrollbar);

	SearchGO(App->scene->root);

	ImGui::End();
}

void ModuleImGui::SearchGO(GameObject* parent)
{
	for (int i = 0; i < parent->go_children.size(); i++)
	{
		if (!parent->go_children[i]->go_active)
		{
			ImGui::PushStyleColor(ImGuiCol_Text, { 0.5f, 0.5f, 0.5f, 1.0f });
		}

		if (parent->go_children[i]->go_children.size() > 0)
		{
			uint flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
			if (parent == App->scene->root)
				flags |= ImGuiTreeNodeFlags_Framed;
			if (parent->go_children[i] == focused_go)
				flags |= ImGuiTreeNodeFlags_Selected;

			bool unfold = ImGui::TreeNodeEx(parent->go_children[i]->go_name.c_str(), flags);
			if (ImGui::IsItemClicked())
				focused_go = parent->go_children[i];
			if (unfold)
			{
				SearchGO(parent->go_children[i]);
				ImGui::TreePop();
			}
		}
		else
		{
			uint flags = ImGuiTreeNodeFlags_Leaf;
			if (parent->go_children[i] == focused_go)
				flags |= ImGuiTreeNodeFlags_Selected;

			ImGui::TreeNodeEx(parent->go_children[i]->go_name.c_str(), flags);
			if (ImGui::IsItemClicked())
				focused_go = parent->go_children[i];
			ImGui::TreePop();
		}
		if (!parent->go_children[i]->go_active)
		{
			ImGui::PopStyleColor();
		}
	}
}

void ModuleImGui::GetHardWareData()
{
	uint64_t totVideoMem, currVideoMem, availbVideoMem, reserVideoMem;

	if (getGraphicsDeviceInfo(nullptr, nullptr, nullptr, &totVideoMem, &currVideoMem, &availbVideoMem, &reserVideoMem)) {
		App->totalVideoMemF = (float)totVideoMem / (1024.0f * 1024.0f);
		App->currentVideoMemF = (float)currVideoMem / (1024.0f * 1024.0f);
		App->availableVideoMemF = (float)availbVideoMem / (1024.0f * 1024.0f);
		App->reservedVideoMemF = (float)reserVideoMem / (1024.0f * 1024.0f);
	}
}

void ModuleImGui::CreateSphere()
{
	

		ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiSetCond_Once);
		ImGui::SetNextWindowPos(ImVec2(600, 100), ImGuiSetCond_Once);
		ImGui::Begin("Create a Sphere", &spherewindow, ImGuiWindowFlags_MenuBar);
		ImGui::InputInt("Radius", &radius);
		ImGui::InputInt("Position X", &x);
		ImGui::InputInt("Position Y", &y);
		ImGui::InputInt("Position Z", &z);

		if (ImGui::SmallButton("Create Sphere"));
		{
			pos.Set(x, y, z);
			App->physics->CreateSphere(pos, radius);
		}
		if (ImGui::SmallButton("Reset"))
		{
			x = y = z = radius = 0;
		}
		if (ImGui::SmallButton("Check Collision"))
		{
			App->physics->GetSphereCollisions();
		}

		ImGui::End();
	
}

void ModuleImGui::CreateTriangle()
{
	ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiSetCond_Once);
	ImGui::SetNextWindowPos(ImVec2(550, 120), ImGuiSetCond_Once);
	ImGui::Begin("Create a Triangle", &trianglewindow, ImGuiWindowFlags_MenuBar);
	ImGui::InputInt("Position A", &a);
	ImGui::InputInt("Position B", &b);
	ImGui::InputInt("Position C", &c);

	if (ImGui::SmallButton("Create Triangle"));
	{
		pos.Set(a, b, c);
		App->physics->CreateTriangle(pos_a,pos_b,pos_c);
	}
	if (ImGui::SmallButton("Reset"))
	{
		a = b = c = 0;
	}

	ImGui::End(); ///

}


update_status ModuleImGui::PostUpdate(float dt)
{
	//ImGui::Render();
	//ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	return UPDATE_CONTINUE;
}

void ModuleImGui::GetConsoleLog(const char* log)
{
	consolelog.appendf(log);
	scrollconsole = true;
}

void ModuleImGui::DrawImgui()
{
	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}

void ModuleImGui::SearchFolder(const char* path)
{
	WIN32_FIND_DATA file;
	HANDLE search_handle = FindFirstFile(path, &file);
	uint id = 0;
	if (search_handle)
	{
		do
		{
			if (file.dwFileAttributes && FILE_ATTRIBUTE_DIRECTORY)	
			{
				// Check type of flie
				std::string name = file.cFileName;
				std::string extension = "";
				for (int i = name.size() - 1; i >= 0; i--)
					if (name[i] == '.' | name[i] == ' ')
						break;
					else
						extension = name[i] + extension;

				if (extension == "fbx" | extension == "FBX" | extension == "dds" | extension == "png" | extension == "jpg")
				{
					//ImGui::PushID(id);
					id++;

					ImGui::TextColored({ 0, 1, 0, 1 }, file.cFileName);
					
					if (ImGui::IsItemHovered())
					{
						ImGui::SameLine(30);
						ImGui::TextColored({ 1, 1, 0, 1 }, file.cFileName);
					}
					if (ImGui::IsItemClicked())
					{
						std::string newpath = path;
						newpath.pop_back();
						newpath += name;
						sprintf_s(selected_file, file.cFileName);
						selected_file_path = newpath;
						selected_file_type = extension;
					}
				}
				else
				{
					if (name != "." && name != "..")
					{
						if (ImGui::TreeNodeEx(file.cFileName))
						{
							std::string newpath = path;
							newpath.pop_back();
							newpath += file.cFileName;
							newpath += "/*";
							SearchFolder(newpath.c_str());// Recursive function to get all subfloders

							ImGui::TreePop();
						}
					}
				}
			}
		} while (FindNextFile(search_handle, &file));
		FindClose(search_handle);
	}
}