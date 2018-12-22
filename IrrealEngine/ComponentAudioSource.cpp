#include "ComponentAudioSource.h"
#include "Application.h"
#include "GameObject.h"

#include "mmgr/mmgr.h"


ComponentAudioSource::ComponentAudioSource(GameObject* gameobject)
{
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