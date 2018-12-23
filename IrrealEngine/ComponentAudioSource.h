#ifndef _COMPONENT_AUDIO_SOURCE_H_
#define _COMPONENT_AUDIO_SOURCE_H_

#include "Component.h"
#include "Wwise.h"

class WwiseGameObject;

class ComponentAudioSource : public Component
{
public:
	ComponentAudioSource(GameObject* gameobject);
	~ComponentAudioSource();

	bool Update() override;
	void SetInspectorInfo() override;

	Value Save(Document::AllocatorType& allocator) const override;
	bool Load(Document& document) override;

public:
	Wwise::WwiseGameObject* sound_go = nullptr;
};

#endif