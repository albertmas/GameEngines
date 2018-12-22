#include "Globals.h"
#include "Application.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleScene.h"
#include "ModuleImGui.h"
#include "GameObject.h"
#include "ComponentCamera.h"
#include "ComponentTransform.h"
#include "Wwise.h"

#include <corecrt_wstring.h>

#include "Wwise/IO/Win32/AkFilePackageLowLevelIOBlocking.h"
#include ".\mmgr\mmgr.h"


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
	//Wwise::LoadBank("Library/Sounds/Forest_soundbank.bnk");
	
	//Wwise::LoadBank("SoundBanks/Test.bnk");

	LoadSoundBank("death_banc");

	/*float3 pos = App->scene->audiolistenerdefault->GetComponent(Component::TRANSFORMATION)->AsTransform()->position;
	Wwise::CreateSoundObj(App->scene->audiolistenerdefault->UUID, App->scene->audiolistenerdefault->go_name.c_str(), pos.x, pos.y, pos.z, true);*/
	//AK::SoundEngine::AddDefaultListener(App->scene->audiolistenerdefault->UUID);

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
	LOG("Freeing Wwise");
	/*delete camera_listener;
	if (soundbank != nullptr) {
		delete soundbank;
	}*/
	
	/*for (std::list<Wwise::WwiseGameObject*>::iterator it = sound_obj.begin(); it != sound_obj.end(); ++it) {

		delete (*it);
	}*/

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

void ModuleAudio::LoadSoundBank(const char* path)
{
	//SoundBank* new_bank = new SoundBank();
	std::string bank_path = AUDIO_DIRECTORY;
	bank_path += path;
	bank_path += AUDIO_EXTENSION;

	Wwise::LoadBank(bank_path.c_str());

	std::string json_file = bank_path.substr(0, bank_path.find_last_of('.')) + ".json"; // Changing .bnk with .json
	/*GetBankInfo(json_file, new_bank);
	soundbanks.push_back(new_bank);
	soundbank = new_bank;
	return new_bank;*/
}

Wwise::WwiseGameObject* ModuleAudio::CreateSoundObject(const char* name, float3 position)
{
	Wwise::WwiseGameObject* ret = Wwise::CreateSoundObj(last_go_id++, name, position.x, position.y, position.z);
	sound_obj.push_back(ret);

	return ret;
}

Wwise::WwiseGameObject* ModuleAudio::CreateListener(const char* name, math::float3 position)
{
	Wwise::WwiseGameObject* ret;

	if (!listener_created) {

		float3 cam_up = App->scene->GetCurCam()->GetFrustum().up;
		float3 cam_front = App->scene->GetCurCam()->GetFrustum().front;
		float3 cam_pos = App->scene->GetCurCam()->transform.position;

		ret = Wwise::CreateSoundObj(0, "Listener", cam_pos.x, cam_pos.y, cam_pos.z, true);
		ret->SetPosition(cam_pos.x, cam_pos.y, cam_pos.z, cam_front.x, cam_front.y, cam_front.z, cam_up.x, cam_up.y, cam_up.z);

		sound_obj.push_back(ret);
		listener_created = true;

	}
	else {
		LOG("It exist a listener already!");
		ret = nullptr;
	}

	return ret;
}