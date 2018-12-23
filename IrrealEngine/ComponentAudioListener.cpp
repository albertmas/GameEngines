#include "ComponentAudioListener.h"
#include "Application.h"
#include "ModuleAudio.h"
#include "GameObject.h"
#include "ComponentTransform.h"

#include "mmgr/mmgr.h"


ComponentAudioListener::ComponentAudioListener(GameObject* gameobject)
{
	my_go = gameobject;
	type = AUDIOLISTENER;
	UUID = pcg32_random_r(&App->rng);

	float3 pos = my_go->GetComponent(Component::TRANSFORMATION)->AsTransform()->position;
	sound_go = Wwise::CreateSoundObj(my_go->UUID, my_go->go_name.c_str(), pos.x, pos.y, pos.z, true);
}

ComponentAudioListener::~ComponentAudioListener()
{
}


bool ComponentAudioListener::Update()
{
	return true;
}

void ComponentAudioListener::SetInspectorInfo()
{
	ImGui::Spacing();
	if (ImGui::CollapsingHeader("Audio Listener", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::InputInt("Volume", &App->audio->volume);
	}
}

Value ComponentAudioListener::Save(Document::AllocatorType& allocator) const
{
	Value CompArray(kObjectType);

	return CompArray;
}

bool ComponentAudioListener::Load(Document& document)
{
	return true;
}