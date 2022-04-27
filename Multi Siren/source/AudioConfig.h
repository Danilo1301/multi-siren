#pragma once

#include "pch.h"

class AudioConfig {
public:
	std::string fileName;
	float volume = 1.0f;
	ISoundSource* soundSource = NULL;

	AudioConfig(std::string fileName);

	void LoadSource();
	bool IsInUse();
	void Destroy();

	Json::Value ToJSON();
	void FromJSON(Json::Value value);
};