#include "ComponentAudioListener.h"
#include "Application.h"
#include "GameObject.h"

#include "mmgr/mmgr.h"


ComponentAudioListener::ComponentAudioListener(GameObject* gameobject)
{
	my_go = gameobject;
	type = AUDIOLISTENER;
	UUID = pcg32_random_r(&App->rng);
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