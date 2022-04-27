#include "AudioConfig.h"

#include "Mod.h"

#include "SirenConfigs.h"

AudioConfig::AudioConfig(std::string fileName) {
	this->fileName = fileName;
}

void AudioConfig::LoadSource() {
	if (soundSource != NULL) return;

	char audioPath[PLUGIN_PATH_MAX];
	sprintf(audioPath, "audios\\%s", this->fileName.c_str());
	soundSource = Mod::m_SoundEngine->addSoundSourceFromFile(PLUGIN_PATH(audioPath));
}

bool AudioConfig::IsInUse() {
	for (auto sirenConfig : SirenConfigs::GetSirenConfigs()) {
		if (sirenConfig->horn == this) return true;

		for (auto audioConfig : sirenConfig->sirens) {
			if (audioConfig == this) return true;
		}
	}
	return false;
}

void AudioConfig::Destroy() {
	Mod::m_SoundEngine->removeSoundSource(soundSource);
	soundSource = NULL;
}

Json::Value AudioConfig::ToJSON() {
	Json::Value value = Json::objectValue;

	value["fileName"] = fileName;
	value["volume"] = volume;

	return value;
}

void AudioConfig::FromJSON(Json::Value value) {
	fileName = value["fileName"].asString();
	volume = value["volume"].asFloat();
}