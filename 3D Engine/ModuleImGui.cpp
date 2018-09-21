#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleImGui.h"
#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_sdl.h"
#include "ImGui\imgui_impl_opengl2.h"
#include "ImGui\imgui_internal.h"


ModuleImGui::ModuleImGui(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleImGui::~ModuleImGui()
{
}


bool ModuleImGui::Init()
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
				if (testwindow)
				{
					testwindow = false;
				}
				else
				{
					testwindow = true;
				}
			}
			
			if (ImGui::MenuItem("Quit", "ESC")) 
			{
				return UPDATE_STOP;
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Console", "1"))
			{
				if (consolewindow)
					consolewindow = false;
				else
					consolewindow = true;
			}

			if (ImGui::MenuItem("Configuration", "4"))
			{
				if (configurationwindow)
					configurationwindow = false;
				else
					configurationwindow = true;
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Homework"))
		{

			if (ImGui::MenuItem("Random Number"))
			{
				if (randomwindow) 
				{
					randomwindow = false;
				
				}
				else
				{
					randomwindow = true;
				}

			}
			if (ImGui::MenuItem("Create Sphere"))
			{
				if (spherewindow)
				{
					spherewindow = false;
				}
				else
				{
					spherewindow = true;
				}

			}
				ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("About"))
		{
			ImGui::Text("3D Engine");
			ImGui::Text("3D Engine in development");
			ImGui::Text("Guillem Arman & Albert Mas");			
			ImGui::Spacing();
			if (ImGui::MenuItem("Link to Repository"))
			{
				App->OpenWeb("https://github.com/albertmas/GameEngines");
			}
			if (ImGui::MenuItem("License"))
			{
				App->OpenWeb("https://github.com/albertmas/GameEngines/blob/master2/LICENSE");
			}

			ImGui::EndMenu();
		}
	
	}

	ImGui::EndMainMenuBar();

	// windows

	if (testwindow) {
		
		ImGui::ShowTestWindow();
	}

	if (randomwindow) {
		ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiSetCond_Once);
		ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiSetCond_Once);
		if (ImGui::Begin("Random Number Generator",&randomwindow, ImGuiWindowFlags_MenuBar))
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
	 if (spherewindow) 
	{
		
		ImGui::SetNextWindowSize(ImVec2(400,200), ImGuiSetCond_Once);
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
	
	// Console
	if (consolewindow)
	{
		ImGui::SetNextWindowSize(ImVec2(600, 300), ImGuiSetCond_Once);
		ImGui::SetNextWindowPos(ImVec2(400, 200), ImGuiSetCond_Once);
		ImGui::Begin("Console", &consolewindow);
		for (int a = 0; a < consolelog.size(); a++)
		{
			ImGui::Text(consolelog[a]);
		}

		ImGui::End();
	}
	// Configuration
	if (configurationwindow)
	{
		ImGui::SetNextWindowSize(ImVec2(600, 300), ImGuiSetCond_Once);
		ImGui::SetNextWindowPos(ImVec2(400, 200), ImGuiSetCond_Once);
		ImGui::Begin("Configuration", &configurationwindow, ImGuiWindowFlags_MenuBar);

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
				}
				if (ImGui::MenuItem("Save"))
				{
					// Save
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		if (ImGui::CollapsingHeader("Application"))
		{
			ImGui::InputText("App Name", "3D Engine", 20);
			ImGui::InputText("Organization", "UPC CITM", 20);
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
		}
		if (ImGui::CollapsingHeader("Window"))
		{
			ImGuiIO& io = ImGui::GetIO();
			/*if (ImGui::Checkbox("Active", ))
			{ }*/
			ImGui::Text("Icon:  *default*");
			ImGui::SameLine();
			ImGui::TextColored({ 255, 0, 0, 1 }, "  NOT FINISHED");
			// Brightness
			if (ImGui::SliderFloat("Brightness", &brightness, 0.0f, 1.0f))
				SDL_SetWindowBrightness(App->window->window, brightness);
			// Window size
			if (ImGui::SliderInt("Width", &App->window->width, 640, 1920)){}
				App->window->SetWinWidth(App->window->width);
			if (ImGui::SliderInt("Height", &height, 480, 1080)){}
				//App->window->SetWinHeight(height);
			// FPS
			ImGui::Text("Refresh Rate: ");
			ImGui::SameLine();
			ImGui::TextColored({ 255, 255, 0, 1 }, "60");
			// Window flag
			//ImGui::CheckboxFlags("Flags", (unsigned int *)&io.ConfigFlags, ImGuiConfigFlags_NavEnableKeyboard);
			if (ImGui::Checkbox("Fullscreen", &fullscreen))
				App->window->SetFullscreen(fullscreen);
			ImGui::SameLine();
			if (ImGui::Checkbox("Resizable", &resizable))
				App->window->SetResizable(resizable);
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Useless on Fullscreen");
			if (ImGui::Checkbox("Borderless", &borderless))
				App->window->SetBorderless(borderless);
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Useless on Fullscreen");
			ImGui::SameLine();
			if (ImGui::Checkbox("Full Desktop", &fulldesktop))
				App->window->SetFullDesktop(fulldesktop);
		}
		if (ImGui::CollapsingHeader("File System"))
		{

		}
		if (ImGui::CollapsingHeader("Input"))
		{

		}
		if (ImGui::CollapsingHeader("Hardware"))
		{

		}
		/*if (ImGui::BeginChild("Application"))
		{
			ImGui::EndChild();
		}*/

		ImGui::End();
	}

	if (aboutwindow)
	{
		ImGui::MenuItem("Names");
		
	}

	return UPDATE_CONTINUE;
}

update_status ModuleImGui::PostUpdate(float dt)
{
	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	return UPDATE_CONTINUE;
}