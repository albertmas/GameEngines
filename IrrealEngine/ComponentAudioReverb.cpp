#include "ComponentAudioReverb.h"
#include "Application.h"
#include "ModuleAudio.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"
#include "ComponentAudioSource.h"
#include "ComponentAudioListener.h"

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
	float3 centre_point = App->scene->train->oriented_BB.CenterPoint();
	float3 max_point = centre_point;
	max_point.x += 30;
	float3 min_point = centre_point;
	min_point.x -= 30;
	if (my_go->oriented_BB.Contains(max_point) || my_go->oriented_BB.Contains(min_point))
	{
		App->scene->train->GetComponent(Component::AUDIOSOURCE)->AsAudioSource()->sound_go->SetAuxiliarySends(1.0f, "env_tunnel", App->scene->audiolistenerdefault->GetComponent(Component::AUDIOLISTENER)->AsAudioListener()->sound_go->GetID());
	}
	else
	{
		//App->scene->train->GetComponent(Component::AUDIOSOURCE)->AsAudioSource()->sound_go->SetAuxiliarySends(0.0f, "Master Audio Bus", App->scene->audiolistenerdefault->GetComponent(Component::AUDIOLISTENER)->AsAudioListener()->sound_go->GetID());
	}

	return true;
}

void ComponentAudioReverb::SetInspectorInfo()
{
	ImGui::Spacing();
	if (ImGui::CollapsingHeader("Audio Reverb", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox("Reverb", &reverb);
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