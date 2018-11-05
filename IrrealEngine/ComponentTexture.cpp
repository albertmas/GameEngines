#include "ComponentTexture.h"
#include "ImGui\imgui.h"

ComponentTexture::ComponentTexture(GameObject* gameobject)
{
	my_go = gameobject;
	type = TEXTURE;
}

ComponentTexture::~ComponentTexture()
{
}


bool ComponentTexture :: Update()
{
	return true;
}

void ComponentTexture::SetInspectorInfo()
{
	if (ImGui::CollapsingHeader("Texture:", ImGuiTreeNodeFlags_DefaultOpen))
	{
		/*ImGui::Checkbox("Active", &comp_active);

		if (texture != nullptr)
		{
			ImGui::TextColored({ 0, 1, 1, 1 }, "Texture Path");
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip(texture->texPath.c_str());
			ImGui::Text("Width: %i", texture->texWidth);
			ImGui::Text("Height: %i", texture->texHeight);

			float winWidth = ImGui::GetWindowContentRegionWidth();
			ImVec2 texSize = { texture->texHeight * winWidth / texture->texWidth, winWidth };
			ImGui::Image((ImTextureID)texture->texture, texSize);
		}
		else
			ImGui::TextColored({ 1, 0, 0, 1 }, "Mesh has no texture");*/
	}
}