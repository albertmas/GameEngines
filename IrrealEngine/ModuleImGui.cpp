#include "Application.h"
#include "ModuleImGui.h"
#include "ImGui\imgui_impl_sdl.h"
#include "ImGui\imgui_impl_opengl2.h"
#include "DeviceId\DeviceId.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
#include "ModulePhysics3D.h"
#include "Assimp/include/version.h"
#include "DevIL\include\il.h"




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
			if (ImGui::MenuItem("Properties", "Shift + 3"))
			{
				propertieswindow = !propertieswindow;
			}
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
					App->OpenWeb("https://github.com/albertmas/GameEngines/tree/master2/3D%20Engine/DeviceId");
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
					App->OpenWeb("https://github.com/albertmas/GameEngines/tree/master2/3D%20Engine/SDL");
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
	if (propertieswindow)PropertiesWindow();
	if (spherewindow)CreateSphere();
	if (trianglewindow)CreateTriangle();
	if (cubewindow)App->renderer3D->CreateCube();
	
	
	// Hotkeys
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN && App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		consolewindow = !consolewindow;
	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN && App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		configurationwindow = !configurationwindow;
	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN && App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		propertieswindow = !propertieswindow;

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

bool ModuleImGui::Save(Document& document, FileWriteStream& fws)
{
	return true;
}

bool ModuleImGui::Load(Document& document)
{
	return true;
}

void ModuleImGui::RandomGenerator()
{
	
		ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiSetCond_Once);
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
				rand_int = ("%i", (int)pcg32_boundedrand_r(&rng, (max_rand_int - min_rand_int + 1)) + min_rand_int);
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
				rand_float = ldexp(pcg32_random_r(&rng), -32);
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
	
		ImGui::SetNextWindowSize(ImVec2(App->window->width - config_width - properties_width - 4, 200), ImGuiSetCond_Once);
		ImGui::SetNextWindowPos(ImVec2(252, App->window->height - 201), ImGuiSetCond_Once);
		ImGui::Begin("Console", &consolewindow, ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoMove);
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
	
	
		ImGui::SetNextWindowSize(ImVec2(300, App->window->height - 22), ImGuiSetCond_Once);
		ImGui::SetNextWindowPos(ImVec2(App->window->width - 301, 21), ImGuiSetCond_Once);
		ImGui::Begin("Configuration", &configurationwindow, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoFocusOnAppearing);

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
			/*if (ImGui::Checkbox("Active", &active))
			{ }*/
			ImGui::Text("Icon:  ");
			ImGui::SameLine();
			if (ImGui::Button(icon_name))
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
					if (ImGui::TreeNode("Assets/"))
					{
						ImGui::TreePop();
					}
					if (ImGui::TreeNode("Library/"))
					{
						if (ImGui::TreeNode("Animations/"))
						{
							ImGui::TreePop();
						}
						if (ImGui::TreeNode("Audio/"))
						{
							ImGui::TreePop();
						}
						if (ImGui::TreeNode("Bones/"))
						{
							ImGui::TreePop();
						}
						if (ImGui::TreeNode("Meshes/"))
						{
							ImGui::TreePop();
						}
						if (ImGui::TreeNode("Scenes/"))
						{
							ImGui::TreePop();
						}
						if (ImGui::TreeNode("Textures/"))
						{
							ImGui::TreePop();
						}

						ImGui::TreePop();
					}
					if (ImGui::TreeNode("Settings/"))
					{
						ImGui::TreePop();
					}
					ImGui::EndChild();
					ImGui::PopID();
					
					ImGui::InputText("", icon_name_new, IM_ARRAYSIZE(icon_name_new), ImGuiInputTextFlags_AutoSelectAll);
					ImGui::SameLine();
					if (ImGui::Button("OK", ImVec2(50.0, 0.0)))
					{
						sprintf_s(icon_name, icon_name_new);
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
			// Brightness
			if (ImGui::SliderFloat("Brightness", &brightness, 0.0f, 1.0f))
				SDL_SetWindowBrightness(App->window->window, brightness);
			// Window size
			if (ImGui::SliderInt("Width", &App->window->width, 640, 1920))
				App->window->SetWinWidth(App->window->width);
			if (ImGui::SliderInt("Height", &App->window->height, 480, 1080))
				App->window->SetWinHeight(App->window->height);
			// FPS
			//ImGui::Text("Refresh Rate: ");
			//ImGui::SameLine();
			//ImGui::TextColored({ 255, 255, 0, 1 }, "60");
			// Window flag
			//ImGui::CheckboxFlags("Flags", (unsigned int *)&io.ConfigFlags, ImGuiConfigFlags_NavEnableKeyboard);
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

		ImGui::End();
}

void ModuleImGui::PropertiesWindow()
{
	ImGui::SetNextWindowSize(ImVec2(250, App->window->height - 22), ImGuiSetCond_Once);
	ImGui::SetNextWindowPos(ImVec2(1, 21), ImGuiSetCond_Once);
	ImGui::Begin("Properties", &propertieswindow, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_HorizontalScrollbar);

	for (std::list<FBXMesh*>::iterator iter = App->renderer3D->meshes.begin(); iter != App->renderer3D->meshes.end(); iter++)
	{
		std::string text = (*iter)->meshName;
		if (ImGui::TreeNodeEx(text.c_str(), ImGuiTreeNodeFlags_Framed))
		{
			ImGui::Text("Path: %s", (*iter)->meshPath.c_str());
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip((*iter)->meshPath.c_str());
			ImGui::Spacing();

			if (ImGui::TreeNode("Transformation"))
			{
				ImGui::InputFloat3("Position", (*iter)->meshPos, 1);
				ImGui::InputFloat3("Rotation", (*iter)->meshRot, 1);
				ImGui::InputInt("Scale", &(*iter)->meshScale);

				ImGui::TreePop();
			}
			ImGui::Spacing();
			if (ImGui::TreeNode("Geometry"))
			{
				ImGui::Text("Triangles: %i", (*iter)->num_triangles);
				ImGui::Text("Vertices: %i", (*iter)->num_vertices);

				ImGui::TreePop();
			}
			ImGui::Spacing();
			if (ImGui::TreeNode("Texture"))// preview
			{
				if (ImGui::TreeNode((*iter)->texName.c_str()))
				{
					ImGui::Text("Path: %s", (*iter)->texPath.c_str());
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip((*iter)->texPath.c_str());
					ImGui::Text("Width: %i", (*iter)->texWidth);
					ImGui::Text("Height: %i", (*iter)->texHeight);

					ImGui::TreePop();
				}

				ImGui::TreePop();
			}
			ImGui::Separator();

			ImGui::TreePop();
		}
	}

	ImGui::End();
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
	ImGui::SetNextWindowPos(ImVec2(600, 100), ImGuiSetCond_Once);
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
