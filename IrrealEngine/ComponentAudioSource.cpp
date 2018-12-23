#include "ComponentAudioSource.h"
#include "Application.h"
#include "ModuleAudio.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "Game\Library\Sounds\Wwise_IDs.h"

#include "mmgr/mmgr.h"


ComponentAudioSource::ComponentAudioSource(GameObject* gameobject)
{
	my_go = gameobject;
	type = AUDIOSOURCE;
	UUID = pcg32_random_r(&App->rng);

	float3 pos = my_go->GetComponent(Component::TRANSFORMATION)->AsTransform()->position;
	sound_go = Wwise::CreateSoundObj(my_go->UUID, my_go->go_name.c_str(), pos.x, pos.y, pos.z);
}

ComponentAudioSource::~ComponentAudioSource()
{
}


bool ComponentAudioSource::Update()
{
	float3 pos = my_go->GetComponent(Component::TRANSFORMATION)->AsTransform()->position;
	sound_go->SetPosition(pos.x, pos.y, pos.z);

	return true;
}

void ComponentAudioSource::SetInspectorInfo()
{
	ImGui::Spacing();
	if (ImGui::CollapsingHeader("Audio Source", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::Button("Play"))
		{
			sound_go->PlayEvent(AK::EVENTS::DEATH);
		}
		if (ImGui::Button("Stop"))
		{
			sound_go->PauseEvent(AK::EVENTS::DEATH);
		}
		ImGui::Text("PosX %f", sound_go->GetPos().x);
		ImGui::Text("PosY %f", sound_go->GetPos().y);
		ImGui::Text("PosZ %f", sound_go->GetPos().z);
	}
}

Value ComponentAudioSource::Save(Document::AllocatorType& allocator) const
{
	Value CompArray(kObjectType);

	return CompArray;
}

bool ComponentAudioSource::Load(Document& document)
{
	return true;
}