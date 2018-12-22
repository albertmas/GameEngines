#ifndef _COMPONENT_AUDIO_LISTENER_H_
#define _COMPONENT_AUDIO_LISTENER_H_

#include "Component.h"


class ComponentAudioListener : public Component
{
public:
	ComponentAudioListener(GameObject* gameobject);
	~ComponentAudioListener();

	bool Update() override;
	void SetInspectorInfo() override;

	Value Save(Document::AllocatorType& allocator) const override;
	bool Load(Document& document) override;
};

#endif