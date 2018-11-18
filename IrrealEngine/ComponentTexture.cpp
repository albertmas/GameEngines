#include "ComponentTexture.h"
#include "Application.h"
#include "ModuleTextureLoader.h"
#include "ModuleImGui.h"
#include "GameObject.h"

#include "ImGui/imgui.h"
#include "mmgr/mmgr.h"


ComponentTexture::ComponentTexture(GameObject* gameobject)
{
	my_go = gameobject;
	type = TEXTURE;
	UUID = pcg32_random_r(&App->rng);
}

ComponentTexture::~ComponentTexture()
{
	RELEASE(texture);
}


bool ComponentTexture::Update()
{
	if (active)
	{
		if (texture != nullptr)
		{
			if (texture->id != 0)
				glBindTexture(GL_TEXTURE_2D, texture->id);
			else
				glColor3f(texture->color.x, texture->color.y, texture->color.z);
		}
	}

	return true;
}

void ComponentTexture::SetInspectorInfo()
{
	ImGui::Spacing();
	if (ImGui::CollapsingHeader("Texture", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::PushID("TextureActive");
		ImGui::Checkbox("Active", &active);
		ImGui::Spacing();
		ImGui::PopID();

		if (texture != nullptr)
		{
			if (texture->tex)
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
			{
				ImGui::TextColored({ 1, 0, 0, 1 }, "Mesh has no texture");
				ImGui::Spacing();

				static float3 defaultColor = texture->color;
				if (ImGui::ColorButton("Default", { defaultColor.x, defaultColor.y, defaultColor.z, 1 }))
				{
					texture->color = defaultColor;
				} ImGui::SameLine();
				if (ImGui::ColorButton("Red", { 1, 0, 0, 1 }))
				{
					texture->color = { 1, 0, 0 };
				} ImGui::SameLine();
				if (ImGui::ColorButton("Green", { 0, 1, 0, 1 }))
				{
					texture->color = { 0, 1, 0 };
				} ImGui::SameLine();
				if (ImGui::ColorButton("Blue", { 0, 0, 1, 1 }))
				{
					texture->color = { 0, 0, 1 };
				} ImGui::SameLine();
				if (ImGui::ColorButton("Pink", { 1, 0, 1, 1 }))
				{
					texture->color = { 1, 0, 1 };
				} ImGui::SameLine();
				if (ImGui::ColorButton("Yellow", { 1, 1, 0, 1 }))
				{
					texture->color = { 1, 1, 0 };
				} ImGui::SameLine();
				if (ImGui::ColorButton("Grey", { 0.5f, 0.5f, 0.5f, 1 }))
				{
					texture->color = { 0.5f, 0.5f, 0.5f };
				}
				ImGui::Spacing();

				if (ImGui::Button("Set Texture"))
				{
					// Create new texture
					// or choose an existing one
				}
			}
		}
	}
}

Value ComponentTexture::Save(Document::AllocatorType& allocator) const
{
	Value CompArray(kObjectType);
	CompArray.AddMember("Type", type, allocator);
	CompArray.AddMember("Active", active, allocator);
	CompArray.AddMember("UUID", UUID, allocator);
	Value name(texture->name.c_str(), allocator);
	CompArray.AddMember("Texture", name, allocator);
	Value ColorArray(kArrayType);
	ColorArray.PushBack(texture->color.x, allocator);
	ColorArray.PushBack(texture->color.y, allocator);
	ColorArray.PushBack(texture->color.z, allocator);
	CompArray.AddMember("Color", ColorArray, allocator);

	return CompArray;
}

bool ComponentTexture::Load(Document& document)
{
	return true;
}