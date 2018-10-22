#include "Panel_Scene.h"
#include "ModuleCamera3D.h"
#include "Application.h"
#include "Texture.h"


Panel_Scene::Panel_Scene() :Panel("Scene")
{
	active = true;
}


Panel_Scene::~Panel_Scene()
{
}

void Panel_Scene::Draw()
{
	if (ImGui::BeginDock("Scene", &active))
	{
		ImVec2 size = ImGui::GetContentRegionAvail();
		glEnable(GL_TEXTURE_2D);
		if (App->camera->SceneT() != nullptr)
		{
			ImGui::Image((void*)App->camera->SceneT()->GetTextureID(), size, ImVec2(0, 1), ImVec2(1, 0));
		}
		glDisable(GL_TEXTURE_2D);
	}
	ImGui::EndDock();

	if (App->camera->SceneT() != nullptr)
	{
		App->camera->SceneT()->Render();
		App->camera->SceneT()->Unbind();
	}
}
