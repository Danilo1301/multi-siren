#pragma once

#include "pch.h"

#include "AudioConfig.h"

class SirenConfig {
public:
	int modelId;
	std::vector<AudioConfig*> sirens;
	AudioConfig* horn = NULL;

	SirenConfig(int modelId);

	void AddSirenAudio(AudioConfig* audioConfig);
	void RemoveSirenAudio(AudioConfig* audioConfig);

	void SetHorn(AudioConfig* audioConfig);
	void RemoveHorn();

	Json::Value ToJSON();
	void FromJSON(Json::Value value);
};