#include "Wwise.h"
#include <assert.h>
#include "Globals.h"
#include "Include_Wwise.h"
#include "Wwise/IO/Win32/AkFilePackageLowLevelIOBlocking.h"
#include "Wwise/IO/Win32/AkDefaultIOHookBlocking.h"
#include "Wwise/SDK/include/AkDefaultIOHookBlocking.h"
#include "Wwise/SDK/include/AkFileHelpers.h"
#include <AK/Plugin/AkRoomVerbFXFactory.h>

//CAkDefaultIOHookBlocking g_defaultIO;
CAkFilePackageLowLevelIOBlocking g_lowLevelIO;

#define BANKNAME_INIT "Library/Sounds/Init.bnk"

//Initialize all Wwise modules. Receives the base path for soundbanks and the current language
bool Wwise::InitWwise()
{
	bool ret = true;

	ret = InitMemoryManager();
	ret = InitStreamManager();
	ret = InitDeviceSettings();
	ret = InitSoundEngine();
	ret = InitMusicEngine();

#ifndef AK_OPTIMIZED
	// Initialize communications (not in release build!)
	AkCommSettings commSettings;
	AK::Comm::GetDefaultInitSettings(commSettings);
	if (AK::Comm::Init(commSettings) != AK_Success)
	{
		assert(!"Could not initialize communication.");
		ret = false;
	}
#endif // AK_OPTIMIZED

	//g_lowLevelIO.SetBasePath(AKTEXT("../../../samples/IntegrationDemo/WwiseProject/GeneratedSoundBanks/Windows/"));
	AK::StreamMgr::SetCurrentLanguage(AKTEXT("English(US)"));
	
	LoadBank(BANKNAME_INIT);

	return ret;
}

bool Wwise::InitMemoryManager()
{
	// Initializing default memory manager
	AkMemSettings memSettings;
	memSettings.uMaxNumPools = 20;

	if (AK::MemoryMgr::Init(&memSettings) != AK_Success)
	{
		assert(!"Could not create the memory manager.");
		return false;
	}

	return true;
}

bool Wwise::InitStreamManager()
{
	// Initializing the Streaming Manager
	AkStreamMgrSettings stmSettings;
	AK::StreamMgr::GetDefaultSettings(stmSettings);

	// Customize the Stream Manager settings here
	if (!AK::StreamMgr::Create(stmSettings))
	{
		assert(!"Could not create the Streaming Manager");
		return false;
	}
	// -------------------
	AkDeviceSettings deviceSettings;
	AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);
	if (g_lowLevelIO.Init(deviceSettings) != AK_Success)
	{
		assert(!"Could not create the streaming device and Low-Level I/O system");
		return false;
	}

	return true;
}

bool Wwise::InitDeviceSettings()
{
	// Initializing the default IO device
	AkDeviceSettings deviceSettings;
	AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);
	if (g_lowLevelIO.Init(deviceSettings) != AK_Success)
	{
		assert(!"Could not create the streaming device and Low-Level I/O system");
		return false;
	}
	else {
		return true;
	}

	return true;
}

bool Wwise::InitSoundEngine()
{
	// Initializing the Sound Engine
	AkInitSettings initSettings;
	AkPlatformInitSettings platformInitSettings;
	AK::SoundEngine::GetDefaultInitSettings(initSettings);
	AK::SoundEngine::GetDefaultPlatformInitSettings(platformInitSettings);

	if (AK::SoundEngine::Init(&initSettings, &platformInitSettings) != AK_Success)
	{
		assert(!"Could not initialize the Sound Engine.");
		return false;
	}

	return true;
}

bool Wwise::InitMusicEngine()
{
	// Initializing the Music Engine
	AkMusicSettings musicInit;
	AK::MusicEngine::GetDefaultInitSettings(musicInit);

	if (AK::MusicEngine::Init(&musicInit) != AK_Success)
	{
		assert(!"Could not initialize the Music Engine.");
		return false;
	}

	return true;
}

bool Wwise::CloseWwise()
{
#ifndef AK_OPTIMIZED
	// Terminate Communication Services
	AK::Comm::Term();
#endif // AK_OPTIMIZED

	// Terminate the music engine
	AK::MusicEngine::Term();

	// Terminate the sound engine
	AK::SoundEngine::Term();

	// Terminate the streaming device and streaming manager
	g_lowLevelIO.Term();
	if (AK::IAkStreamMgr::Get())
		AK::IAkStreamMgr::Get()->Destroy();

	// Terminate the Memory Manager
	AK::MemoryMgr::Term();

	return true;
}

bool Wwise::LoadBank(const char* name)
{
	unsigned long bankID;
	AKRESULT eResult = AK::SoundEngine::LoadBank(name, AK_DEFAULT_POOL_ID, bankID);
	if (eResult != AK_Success)
	{
		LOG("Could not load soundbank!");
		return false;
	}

	return true;
}

void Wwise::SoundObject::PlayEvent(const char* name)
{
	AK::SoundEngine::PostEvent(name, SoundID);
	LOG("Playing event: %s", name);
}