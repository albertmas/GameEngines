#include "ComponentAudioReverb.h"
#include "Application.h"
#include "ModuleAudio.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"

#include "mmgr/mmgr.h"


ComponentAudioReverb::ComponentAudioReverb(GameObject* gameobject)
{
	my_go = gameobject;
	type = AUDIOREVERB;
	UUID = pcg32_random_r(&App->rng);
}

ComponentAudioReverb::~ComponentAudioReverb()
{
}


bool ComponentAudioReverb::Update()
{

	return true;
}

void ComponentAudioReverb::SetInspectorInfo()
{
	ImGui::Spacing();
	if (ImGui::CollapsingHeader("Audio Reverb", ImGuiTreeNodeFlags_DefaultOpen))
	{

	}
}

Value ComponentAudioReverb::Save(Document::AllocatorType& allocator) const
{
	Value CompArray(kObjectType);

	return CompArray;
}

bool ComponentAudioReverb::Load(Document& document)
{
	return true;
}