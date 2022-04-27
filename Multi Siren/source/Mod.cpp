#include "Mod.h"

#include "Vehicles.h"
#include "AudioConfigs.h"
#include "SirenConfigs.h"
#include "CMenuManager.h"
#include "Config.h"

#include "windows/WindowMain.h"

bool Mod::m_DebugEnabled = false;
std::string Mod::m_Version = "1.0.0";
std::string Mod::m_Name = "Multi Siren";
ISoundEngine* Mod::m_SoundEngine = nullptr;
float Mod::m_AudioDistanceMult = 1.0f;
float Mod::m_Volume = 1.0f;

void Mod::Init() {
	Log::file << "[Mod] Init" << std::endl;

	m_SoundEngine = irrklang::createIrrKlangDevice();

	Log::file << "[Mod] Sound engine " << (m_SoundEngine ? "loaded" : "error") << std::endl;
	
	Config::LoadJSON();

	Config::SaveJSON();
}

void Mod::Update() {
	Input::Update();

	if (Input::GetKey(17) && Input::GetKey(16)) {
		if (Input::GetKeyDown(68)) {
			m_DebugEnabled = !m_DebugEnabled;
			CMessages::AddMessageJumpQ(m_DebugEnabled ? "Debug enabled" : "Debug disabled", 1000, 0, false);
		}
	}

	if (Input::GetKey(17)) {
		if (Input::GetKeyDown(77)) {
			ToggleMenu();
		}
	}

	Menu::Update();
	Vehicles::TryAddAllVehicles();

	vec3df camPos(TheCamera.GetPosition().x * m_AudioDistanceMult, TheCamera.GetPosition().z * m_AudioDistanceMult, TheCamera.GetPosition().y * m_AudioDistanceMult);
	m_SoundEngine->setListenerPosition(camPos, vec3df(TheCamera.m_matrix->right.y, TheCamera.m_matrix->up.z, -TheCamera.m_matrix->right.x), vec3df(0, 0, 0), vec3df(0, 1, 0));
	m_SoundEngine->setSoundVolume(GetVolume());

	for (auto pair : Vehicles::m_Vehicles)
	{
		Vehicle* vehicle = pair.second;
		vehicle->Update();
	}
}

void Mod::Draw() {
	for (auto pair : Vehicles::m_Vehicles)
	{
		Vehicle* vehicle = pair.second;
		vehicle->Draw();
		if (m_DebugEnabled) vehicle->DrawDebug();
		bool isPlayerVeh = vehicle->m_Vehicle == FindPlayerVehicle(0, false);
	}
	Menu::Draw();

	if (m_DebugEnabled) {
		Menu::DrawString("SFX Volume= " + std::to_string((int)FrontEndMenuManager.m_nSfxVolume), 50, 200, CRGBA(255, 255, 255));
		Menu::DrawString("Volume= " + std::to_string(m_Volume), 50, 220, CRGBA(255, 255, 255));
		Menu::DrawString("AudioDistanceMult= " + std::to_string(m_AudioDistanceMult), 50, 240, CRGBA(255, 255, 255));
		Menu::DrawString("EndVolume= " + std::to_string(GetVolume()), 50, 260, CRGBA(255, 255, 255));
	}
}

float Mod::GetVolume() {
	return m_Volume; //* ((float)FrontEndMenuManager.m_nSfxVolume / (float)64);
}

void Mod::MuteSounds() {
	if(m_SoundEngine) m_SoundEngine->setSoundVolume(0);
}

void Mod::ToggleMenu() {
	if (!Menu::m_Visible) {
		WindowMain::m_Vehicle = FindPlayerVehicle(0, false);
		if (!WindowMain::m_Vehicle) {
			CMessages::AddMessageJumpQ("You must be in a vehicle", 1000, 0, false);
			return;
		}
	}

	Menu::m_Visible = !Menu::m_Visible;
	Menu::m_Hide = false;

	if (Menu::m_Visible) {
		WindowMain::Create();
		SetPlayerControl(false);
	}
	else {
		Menu::RemoveAllWindows();
		SetPlayerControl(true);

		Config::SaveJSON();
	}
}

void Mod::SetPlayerControl(bool enabled) {
	Command<Commands::SET_PLAYER_CONTROL>(0, enabled);
}

Mod::Mod() {
	Log::Open("Multi Siren");
	Log::file << "Initialized" << std::endl;

	Menu::m_DefaultPosition.y += 110.0f;

	Events::initGameEvent += Init;
	Events::processScriptsEvent += Update;
	Events::drawingEvent += Draw;

	Events::drawMenuBackgroundEvent.Add(MuteSounds);
	Events::onPauseAllSounds.AddAfter(MuteSounds);
	Events::d3dLostEvent.AddAfter(MuteSounds);
	Events::d3dResetEvent.AddAfter(MuteSounds);

	Events::vehicleSetModelEvent.after += [](CVehicle* vehicle, int modelId) {
		//if (!Vehicles::HasVehicle(vehicle)) Vehicles::AddVehicle(vehicle);
	};

	Events::vehicleDtorEvent.before += [](CVehicle* vehicle) {
		if (Vehicles::HasVehicle(vehicle)) Vehicles::RemoveVehicle(vehicle);
	};
}

Mod mod;