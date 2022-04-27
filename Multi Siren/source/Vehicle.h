#pragma once

#include "pch.h"
#include "AudioConfig.h"
#include "SirenConfig.h"

class Vehicle {
public:
	CVehicle* m_Vehicle;
	ISound* m_Sound = NULL;
	AudioConfig* m_AudioConfg = NULL;
	SirenConfig* m_SirenConfig;

	CObject* m_TestObject = NULL;

	bool m_PrevSirenState = false;
	bool m_PrevHornState = false;

	int m_CurrentSirenIndex = 0;

	Vehicle(CVehicle* veh);

	void Update();
	void UpdateSound();
	void UpdateInputs();
	void ChangeSirenSound(int by = 0);
	void Draw();
	void DrawDebug();
	void Destroy();

	bool GetSirenState();
	bool GetHornState();

	void RemoveCurrentAudioConfig();
	void SetAudioConfig(AudioConfig* audioConfig);
};