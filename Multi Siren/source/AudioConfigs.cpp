#include "AudioConfigs.h"

std::vector<AudioConfig*> AudioConfigs::m_AudioConfigs;

AudioConfig* AudioConfigs::AddAudioConfig(std::string fileName) {
	Log::file << "[AudioConfigs] Added '" << "' '" << fileName << "'" << std::endl;

	AudioConfig* audioConfig = new AudioConfig(fileName);
	audioConfig->LoadSource();
	m_AudioConfigs.push_back(audioConfig);
	return audioConfig;
}

bool AudioConfigs::TryRemoveAudioConfig(AudioConfig* audioConfig) {
	Log::file << "[AudioConfigs] TryRemoveAudioConfig '" << audioConfig->fileName << std::endl;

	if (audioConfig->IsInUse()) {
		return false;
	}

	RemoveAudioConfig(audioConfig);

	return true;
}

void AudioConfigs::RemoveAudioConfig(AudioConfig* audioConfig) {
	Log::file << "[AudioConfigs] RemoveAudioConfig '" << audioConfig->fileName << std::endl;

	

	Log::file << "[AudioConfigs] Erase '" << audioConfig->fileName << std::endl;
	auto it = std::find(m_AudioConfigs.begin(), m_AudioConfigs.end(), audioConfig);
	m_AudioConfigs.erase(it);

	Log::file << "[AudioConfigs] Destroy '" << audioConfig->fileName << std::endl;
	audioConfig->Destroy();

	Log::file << "[AudioConfigs] Delete '" << audioConfig->fileName << std::endl;
	delete audioConfig;
}

int AudioConfigs::GetAudioConfigIndex(AudioConfig* audioConfig) {
	auto it = std::find(m_AudioConfigs.begin(), m_AudioConfigs.end(), audioConfig);
	auto index = it - m_AudioConfigs.begin();
	return index;
}
AudioConfig* AudioConfigs::GetAudioConfigFromIndex(int index) {
	if (index >= (int)m_AudioConfigs.size()) return nullptr;
	return m_AudioConfigs[index];
}