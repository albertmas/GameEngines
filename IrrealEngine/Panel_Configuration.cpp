#include "Panel_Configuration.h"
#include "ModuleWindow.h"
#include "stdio.h"




Panel_Configuration::Panel_Configuration() :Panel("Configuration")
{
	ini_pos_x = 800;
	ini_pos_y = 20;
	ini_width = 475;
	ini_height = 800;
	render_pos = { 800,20 };
	render_size = { 800,20 };
}


Panel_Configuration::~Panel_Configuration()
{

}

void Panel_Configuration::Draw()
{
	if (ImGui::BeginDock("Configuration", &active))
	{
		render_pos = ImGui::GetWindowPos();
		render_size = ImGui::GetWindowSize();
		Application();
		for (int i = 0; App->GetModule(i) != nullptr; i++)
		{
			App->GetModule(i)->DrawModuleConfig();
			
		}
	}
	ImGui::EndDock();

}

void Panel_Configuration::Application()
{
	if (ImGui::CollapsingHeader("Application"))
	{
		
		static char app_name[128] = TITLE;
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::InputText("App Name", app_name, IM_ARRAYSIZE(app_name));
		

		std::string window_title;
		window_title.append(app_name);
		window_title.append(" - ");
		


		App->window->SetTitle(window_title.c_str());
		

		static int i = 32;
		if (fps_log[IM_ARRAYSIZE(fps_log)] != ImGui::GetIO().Framerate)
		{

			if (i > 0)
			{
				fps_log[i - 1] = fps_log[i];
				i--;
			}
			else
			{
				i = IM_ARRAYSIZE(fps_log);
			}
			fps_log[IM_ARRAYSIZE(fps_log) - 1] = ImGui::GetIO().Framerate;
		}

		static char tmp_string[4096];
		sprintf_s(tmp_string, 4096, "Framerate: %.2f", ImGui::GetIO().Framerate);
		ImGui::PlotHistogram("##framerate", fps_log, IM_ARRAYSIZE(fps_log), 0, tmp_string, 0.0f, 120.0f, ImVec2(310, 100));

		static int i2 = 32;
		float mss = (1000.0f / ImGui::GetIO().Framerate);
		if (mms_log[IM_ARRAYSIZE(mms_log)] != mss)
		{
			if (i2 > 0)
			{
				mms_log[i2 - 1] = mms_log[i2];
				i2--;
			}
			else
			{
				i2 = IM_ARRAYSIZE(mms_log);
			}
			mms_log[IM_ARRAYSIZE(mms_log)] = mss;
		}

		static char tmp_string2[4096];
		sprintf_s(tmp_string2, 4096, "Miliseconds: %.3f", mss);
		ImGui::PlotHistogram("##miliseconds", mms_log, IM_ARRAYSIZE(mms_log), 0, tmp_string2, 0.0f, 120.0f, ImVec2(310, 100));
	}
}

