#include "Application.h"
#include "ModuleImGui.h"
#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_sdl.h"
#include "ImGui\imgui_impl_opengl2.h"


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
	//ImGui::DestroyContext();

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
	if (ImGui::BeginMainMenuBar()) {

		if (ImGui::BeginMenu("Menu")) {


			if (ImGui::MenuItem("Show Window")) {
				if (testwindow) {
					testwindow = false;
				}
				else
				{
					testwindow = true;
				}
			}
			

			if (ImGui::MenuItem("Close Application")) {
				return UPDATE_STOP;
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Homework")) {

			if (ImGui::MenuItem("Random Number")) {
				if (randomwindow) {
					randomwindow = false;
				}
				else
				{
					randomwindow = true;
				}
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
		ImGui::Text("Random Number Generator");
		ImGui::SmallButton("Generate Random number");
		ImGui::InputInt("Number", &number);
	}
	if (testwindow2) {
		ImGui::ShowStyleEditor();
	}
	return UPDATE_CONTINUE;
}

update_status ModuleImGui::PostUpdate(float dt)
{
	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	return UPDATE_CONTINUE;
}