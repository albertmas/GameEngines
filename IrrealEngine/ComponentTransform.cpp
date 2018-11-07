#include "ComponentTransform.h"
#include "ImGui/imgui.h"

ComponentTransform::ComponentTransform(GameObject* gameobject)
{
	my_go = gameobject;
	type = TRANSFORMATION;
}

ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::SetInspectorInfo()
{
	ImGui::Spacing();
	if (ImGui::CollapsingHeader("Transformation"), ImGuiTreeNodeFlags_DefaultOpen)
	{
		ImGui::TextColored({ 0, 1, 1, 1 }, "Position:");
		ImGui::Text("X: %.3f | Y: %.3f | Z: %.3f", position.x, position.y, position.z);
		ImGui::Separator();

		ImGui::TextColored({ 0, 1, 1, 1 }, "Rotation:");
		float3 rot = rotation.ToEulerXYZ();
		rot *= 180 / pi;
		ImGui::Text("X: %.3f | Y: %.3f | Z: %.3f", rot.x, rot.y, rot.z);
		ImGui::Separator();

		ImGui::TextColored({ 0, 1, 1, 1 }, "Scale:");
		ImGui::Text("X: %.3f | Y: %.3f | Z: %.3f", scale.x, scale.y, scale.z);

		/*float pos[3] = { position.x, position.y, position.z };
		if (ImGui::InputFloat3("Position", pos, 1))
		{
			position.x = pos[1];
			position.y = pos[2];
			position.z = pos[3];
		}

		float3 euler_deg_rot = rotation.ToEulerXYZ();
		euler_deg_rot *= 180 / pi;
		float rot[3] = { euler_deg_rot.x, euler_deg_rot.y, euler_deg_rot.z };
		ImGui::InputFloat3("Rotation", rot, 1);
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Rotation of the GameObject");

		float _scale[3] = { scale.x, scale.y, scale.z };
		ImGui::InputFloat3("Scale", _scale, 1);
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Rotation of the mesh");*/

	}
}

bool ComponentTransform::Save(Document& document, FileWriteStream& fws) const
{
	Document::AllocatorType& allocator = document.GetAllocator();
	// Save stuff
	Writer<FileWriteStream> writer(fws);
	return true;
}

bool ComponentTransform::Load(Document& document)
{
	return true;
}