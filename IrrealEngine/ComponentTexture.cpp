#include "ComponentTexture.h"
#include "ModuleTextureLoader.h"
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
	ImGui::Spacing();
	if (ImGui::CollapsingHeader("Texture", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::PushID(1);
		ImGui::Checkbox("Active", &active);
		ImGui::Spacing();
		ImGui::PopID();

		if (texture != nullptr)
		{
			ImGui::TextColored({ 0, 1, 1, 1 }, "Texture Path");
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip(texture->path.c_str());
			ImGui::Separator();

			ImGui::Text("Width: %i", texture->width);
			ImGui::Text("Height: %i", texture->height);

			float windowWidth = ImGui::GetWindowContentRegionWidth();
			ImVec2 texSize = { texture->height * windowWidth / texture->width, windowWidth };
			ImGui::Image((ImTextureID)texture->id, texSize);
		}
		else
			ImGui::TextColored({ 1, 0, 0, 1 }, "Mesh has no texture");
	}
}

bool ComponentTexture::Save(Document& document, FileWriteStream& fws) const
{
	Document::AllocatorType& allocator = document.GetAllocator();
	// Save stuff
	Writer<FileWriteStream> writer(fws);
	return true;
}

bool ComponentTexture::Load(Document& document)
{
	return true;
}