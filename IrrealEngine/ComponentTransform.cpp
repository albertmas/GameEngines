#include "ComponentTransform.h"
#include "GameObject.h"
#include "ImGui/imgui.h"

ComponentTransform::ComponentTransform(GameObject* gameobject)
{
	my_go = gameobject;
	type = TRANSFORMATION;
}

ComponentTransform::~ComponentTransform()
{
}


bool ComponentTransform::Update()
{
	return true;
}

void ComponentTransform::SetInspectorInfo()
{
	ImGui::Spacing();
	if (ImGui::CollapsingHeader("Transformation"), ImGuiTreeNodeFlags_DefaultOpen)
	{
		//uint flags = ImGuiInputTextFlags_EnterReturnsTrue;
		if (my_go->go_static)
		{
			//flags |= ImGuiInputTextFlags_ReadOnly;
			ImGui::PushStyleColor(ImGuiCol_Text, { 0.5f, 0.5f, 0.5f, 1.0f });
		}

		ImGui::TextColored({ 0, 1, 1, 1 }, "   X       Y       Z");

		ImGui::PushItemWidth(50);

		// Position
		ImGui::PushID("PosX");
		if (ImGui::DragFloat("", &position.x))
			matrix_local.Set(float4x4::FromTRS(position, rotation, scale));
		ImGui::PopID();

		ImGui::SameLine();
		ImGui::PushID("PosY");
		if (ImGui::DragFloat("", &position.y))
			matrix_local.Set(float4x4::FromTRS(position, rotation, scale));
		ImGui::PopID();

		ImGui::SameLine();
		ImGui::PushID("PosZ");
		if (ImGui::DragFloat("", &position.z))
			matrix_local.Set(float4x4::FromTRS(position, rotation, scale));
		ImGui::PopID();

		ImGui::SameLine();
		ImGui::TextColored({ 0, 1, 1, 1 }, "Position");

		// Rotation
		float3 euler_deg_rot = rotation.ToEulerXYZ();
		euler_deg_rot *= RADTODEG;
		ImGui::PushID("RotX");
		if (ImGui::DragFloat("", &euler_deg_rot.x))
		{
			euler_deg_rot *= DEGTORAD;
			rotation = Quat::FromEulerXYZ(euler_deg_rot.x, euler_deg_rot.y, euler_deg_rot.z);
			matrix_local.Set(float4x4::FromTRS(position, rotation, scale));
		}
		ImGui::PopID();

		ImGui::SameLine();
		ImGui::PushID("RotY");
		if (ImGui::DragFloat("", &euler_deg_rot.y))
		{
			euler_deg_rot *= DEGTORAD;
			rotation = Quat::FromEulerXYZ(euler_deg_rot.x, euler_deg_rot.y, euler_deg_rot.z);
			matrix_local.Set(float4x4::FromTRS(position, rotation, scale));
		}
		ImGui::PopID();

		ImGui::SameLine();
		ImGui::PushID("RotZ");
		if (ImGui::DragFloat("", &euler_deg_rot.z))
		{
			euler_deg_rot *= DEGTORAD;
			rotation = Quat::FromEulerXYZ(euler_deg_rot.x, euler_deg_rot.y, euler_deg_rot.z);
			matrix_local.Set(float4x4::FromTRS(position, rotation, scale));
		}
		ImGui::PopID();

		ImGui::SameLine();
		ImGui::TextColored({ 0, 1, 1, 1 }, "Rotation");

		// Scale
		ImGui::PushID("ScaleX");
		if (ImGui::DragFloat("", &scale.x, 0.05f))
			matrix_local.Set(float4x4::FromTRS(position, rotation, scale));
		ImGui::PopID();

		ImGui::SameLine();
		ImGui::PushID("ScaleY");
		if (ImGui::DragFloat("", &scale.y, 0.05f))
			matrix_local.Set(float4x4::FromTRS(position, rotation, scale));
		ImGui::PopID();

		ImGui::SameLine();
		ImGui::PushID("ScaleZ");
		if (ImGui::DragFloat("", &scale.z, 0.05f))
			matrix_local.Set(float4x4::FromTRS(position, rotation, scale));
		ImGui::PopID();

		ImGui::SameLine();
		ImGui::TextColored({ 0, 1, 1, 1 }, "Scale");

		ImGui::PopItemWidth();

		// Local Matrix
		ImGui::Spacing();
		ImGui::TextColored({ 0, 1, 1, 1 }, "Local Matrix:");
		ImGui::Text("%.3f | %.3f | %.3f | %.3f", matrix_local[0][0], matrix_local[1][0], matrix_local[2][0], matrix_local[3][0]);
		ImGui::Text("%.3f | %.3f | %.3f | %.3f", matrix_local[0][1], matrix_local[1][1], matrix_local[2][1], matrix_local[3][1]);
		ImGui::Text("%.3f | %.3f | %.3f | %.3f", matrix_local[0][2], matrix_local[1][2], matrix_local[2][2], matrix_local[3][2]);
		ImGui::Text("%.3f | %.3f | %.3f | %.3f", matrix_local[0][3], matrix_local[1][3], matrix_local[2][3], matrix_local[3][3]);

		if (my_go->go_static)
			ImGui::PopStyleColor();
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