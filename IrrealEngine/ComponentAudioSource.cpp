#include "ComponentAudioSource.h"
#include "Application.h"
#include "GameObject.h"
#include "ModuleAudio.h"
#include "Wwise.h"

#include "mmgr/mmgr.h"


ComponentAudioSource::ComponentAudioSource(GameObject* gameobject)
{
	my_go = gameobject;
	type = AUDIOSOURCE;
	UUID = pcg32_random_r(&App->rng);
}

ComponentAudioSource::~ComponentAudioSource()
{
}


bool ComponentAudioSource::Update()
{
	return true;
}

void ComponentAudioSource::SetInspectorInfo()
{
	ImGui::Spacing();
	if (ImGui::CollapsingHeader("Audio Source", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::Button("Play"))
		{

		}
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