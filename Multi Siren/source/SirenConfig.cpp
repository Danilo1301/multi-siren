#include "SirenConfig.h"

#include "AudioConfigs.h"

SirenConfig::SirenConfig(int modelId) {
	this->modelId = modelId;
}

void SirenConfig::AddSirenAudio(AudioConfig* audioConfig) {
	Log::file << "[SirenConfig] AddSirenAudio '" << audioConfig->fileName << std::endl;

	sirens.push_back(audioConfig);
}

void SirenConfig::RemoveSirenAudio(AudioConfig* audioConfig) {
	Log::file << "[SirenConfig] RemoveSirenAudio '" << audioConfig->fileName << std::endl;

	auto it = std::find(sirens.begin(), sirens.end(), audioConfig);
	sirens.erase(it);

	AudioConfigs::TryRemoveAudioConfig(audioConfig);
}

void SirenConfig::SetHorn(AudioConfig* audioConfig) {
	Log::file << "[SirenConfig] SetHorn '" << audioConfig->fileName << std::endl;

	horn = audioConfig;
}

void SirenConfig::RemoveHorn() {
	auto audioConfig = horn;

	Log::file << "[SirenConfig] RemoveHorn '" << audioConfig->fileName << std::endl;

	horn = NULL;

	AudioConfigs::TryRemoveAudioConfig(audioConfig);
}

Json::Value SirenConfig::ToJSON() {
	Json::Value value = Json::objectValue;

	value["modelId"] = modelId;

	if (horn) {
		value["horn"] = AudioConfigs::GetAudioConfigIndex(horn);
	}
	else {
		value["horn"] = -1;
	}

	value["sirens"] = Json::arrayValue;
	for (auto siren : sirens) {
		value["sirens"].append(AudioConfigs::GetAudioConfigIndex(siren));
	}

	return value;
}

void SirenConfig::FromJSON(Json::Value value) {
	modelId = value["modelId"].asInt();

	if (value["horn"].asInt() != -1) {
		auto audioConfig = AudioConfigs::GetAudioConfigFromIndex(value["horn"].asInt());

		if(audioConfig) horn = audioConfig;
	}

	for (size_t i = 0; i < value["sirens"].size(); i++)
	{
		auto index = value["sirens"][i].asInt();
		auto audioConfig = AudioConfigs::GetAudioConfigFromIndex(index);

		if(audioConfig) sirens.push_back(audioConfig);
	}
}