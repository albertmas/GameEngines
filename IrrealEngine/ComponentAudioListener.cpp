#include "ComponentAudioListener.h"
#include "Application.h"
#include "GameObject.h"

#include "mmgr/mmgr.h"


ComponentAudioListener::ComponentAudioListener(GameObject* gameobject)
{
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