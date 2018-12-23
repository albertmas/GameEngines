#ifndef _COMPONENT_AUDIO_REVERB_H_
#define _COMPONENT_AUDIO_REVERB_H_

#include "Component.h"
#include "Wwise.h"


class ComponentAudioReverb : public Component
{
public:
	ComponentAudioReverb(GameObject* gameobject);
	~ComponentAudioReverb();

	bool Update() override;
	void SetInspectorInfo() override;

	Value Save(Document::AllocatorType& allocator) const override;
	bool Load(Document& document) override;
};

#endif