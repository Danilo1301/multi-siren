#include "Vehicle.h"
#include "Mod.h"
#include "AudioConfigs.h"
#include "SirenConfigs.h"

bool testObject = false;

Vehicle::Vehicle(CVehicle* veh) {
	m_Vehicle = veh;

	UpdateSound();

	m_SirenConfig = SirenConfigs::GetSirenConfig(veh->m_nModelIndex);

	if (testObject && !Command<Commands::HAS_MODEL_LOADED>(3786)) Command<Commands::REQUEST_MODEL>(3786); //missile_05_SFXR
}

void Vehicle::Update() {
	if (testObject && !m_TestObject && Command<Commands::HAS_MODEL_LOADED>(3786)) {
		Command<Commands::CREATE_OBJECT>(3786, 0.0f, 0.0f, 0.0f, &m_TestObject);
		Command<Commands::SET_OBJECT_COLLISION>(m_TestObject, false);
		Command<Commands::ATTACH_OBJECT_TO_CAR>(m_TestObject, m_Vehicle, 0.0f, 0.0f, 1.5f, 0.0f, 0.0f, 0.0f);
	}

	if (GetSirenState() != m_PrevSirenState)
	{
		m_PrevSirenState = GetSirenState();
	}

	if (GetSirenState()) {
		if (!m_AudioConfg) {
			m_CurrentSirenIndex = 0;
			ChangeSirenSound();
		}

		if (m_PrevHornState != GetHornState()) {
			m_PrevHornState = GetHornState();
			ChangeSirenSound();
		}
	}
	else {
		if (m_AudioConfg) {
			RemoveCurrentAudioConfig();
		}
	}

	UpdateInputs();
	UpdateSound();
}

void Vehicle::UpdateSound() {
	vec3df pos3d(m_Vehicle->GetPosition().x * Mod::m_AudioDistanceMult, m_Vehicle->GetPosition().z * Mod::m_AudioDistanceMult, m_Vehicle->GetPosition().y * Mod::m_AudioDistanceMult);

	if (m_AudioConfg) {
		if (!m_Sound) {
			m_Sound = Mod::m_SoundEngine->play3D(m_AudioConfg->soundSource, vec3df(0, 0, 0), true, false, true);
			//Log::file << "[Vehicle] Sound '" << m_AudioConfg->fileName << "'" << (m_Sound ? "loaded" : "error") << std::endl;
		}
	}
	else {
		if (m_Sound) {
			m_Sound->stop();
			m_Sound->drop();
			m_Sound = NULL;
		}
	}

	if (m_Sound) {
		m_Sound->setPosition(pos3d);
		m_Sound->setMinDistance(7.0f * Mod::m_AudioDistanceMult);
		if (m_AudioConfg) {
			m_Sound->setVolume(m_AudioConfg->volume);
		}
	}
}

void Vehicle::UpdateInputs() {
	bool isPlayerVeh = FindPlayerVehicle(0, false) == m_Vehicle;

	if (!isPlayerVeh) return;

	if (Input::GetKey(17)) {
		if (Input::GetKeyDown(71)) {
			ChangeSirenSound(1);
		}
	}
}

void Vehicle::ChangeSirenSound(int by) {
	if (by < 0) return;

	int numSirens = m_SirenConfig->sirens.size();

	m_CurrentSirenIndex += by;
	if (m_CurrentSirenIndex >= numSirens) m_CurrentSirenIndex = 0;


	if (GetHornState()) {
		SetAudioConfig(m_SirenConfig->horn);
	}
	else {
		if (numSirens == 0) {
			RemoveCurrentAudioConfig();
			return;
		}

		SetAudioConfig(m_SirenConfig->sirens[m_CurrentSirenIndex]);
	}
}

void Vehicle::Draw() {

}

void Vehicle::DrawDebug() {
	char text[512];
	auto screenPos = WorldToScreen(m_Vehicle->GetPosition() + CVector(0, 0, -0.5f));

	sprintf(text, "[vehicle] %s %d - siren=%d horn=%d", m_TestObject ? "HASOBJECT" : "NOOBJECT", m_CurrentSirenIndex, GetSirenState(), GetHornState());
	DrawScreenText(text, screenPos);
	screenPos.y += 20.0f;
}

void Vehicle::Destroy() {
	RemoveCurrentAudioConfig();
}

bool Vehicle::GetSirenState() {
	return m_Vehicle->m_nVehicleFlags.bSirenOrAlarm;
}

bool Vehicle::GetHornState() {
	return m_Vehicle->m_nHornCounter > 0;
}

void Vehicle::RemoveCurrentAudioConfig() {
	m_AudioConfg = NULL;

	UpdateSound();
}

void Vehicle::SetAudioConfig(AudioConfig* audioConfig) {
	RemoveCurrentAudioConfig();

	m_AudioConfg = audioConfig;

	UpdateSound();
}