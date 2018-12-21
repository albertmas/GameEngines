#include "Globals.h"
#include "Application.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleCamera3D.h"
#include "ModuleImGui.h"
#include "GameObject.h"
#include "Wwise.h"

#include <corecrt_wstring.h>

#include "Wwise/IO/Win32/AkFilePackageLowLevelIOBlocking.h"
#include "rapidjson\rapidjson.h"
#include ".\mmgr\mmgr.h"

#define BANK_BASE_PATH "SoundBanks/"


ModuleAudio::ModuleAudio(bool start_enabled) : Module(start_enabled)
{
	//name
}

ModuleAudio::~ModuleAudio()
{

}

bool ModuleAudio::Init(Document& document)
{
	LOG("Initializing Wwise");

	//std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	//std::wstring base_path = converter.from_bytes("SoundBanks");

	bool ret = Wwise::InitWwise();
	
	//Wwise::LoadBank("SoundBanks/Test.bnk");

	//LoadSoundBank("Blend.bnk");


	return ret;
}

bool ModuleAudio::Start()
{
	return true;
}

update_status ModuleAudio::PreUpdate(float dt)
{
	if (!muted) {
		SetVolume("Volume", volume);
	}
	else {
		SetVolume("Volume", 0);
	}

	return UPDATE_CONTINUE;
}


update_status ModuleAudio::PostUpdate(float dt)
{

	AK::SoundEngine::RenderAudio();

	return UPDATE_CONTINUE;
}

bool ModuleAudio::CleanUp()
{
	LOG("Unloading Wwished library");
	delete camera_listener;
	if (soundbank != nullptr) {
		delete soundbank;
	}
	
	return Wwise::CloseWwise();

	return true;
}

bool ModuleAudio::Save(Document& document, FileWriteStream& fws)const
{
	bool ret = true;

	return ret;
}

bool ModuleAudio::Load(Document& document)
{
	bool ret = true;

	return ret;
}

// -----------------------------

void ModuleAudio::SetVolume(const char * rtpc, float value)
{
	AK::SoundEngine::SetRTPCValue(rtpc, value);
}
