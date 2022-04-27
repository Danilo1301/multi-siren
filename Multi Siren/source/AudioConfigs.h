#pragma once

#include "pch.h"

#include "AudioConfig.h"

class AudioConfigs {
public:
	static std::vector<AudioConfig*> m_AudioConfigs;

	static AudioConfig* AddAudioConfig(std::string fileName);
	static bool TryRemoveAudioConfig(AudioConfig* audioConfig);
	static void RemoveAudioConfig(AudioConfig* audioConfig);

	static int GetAudioConfigIndex(AudioConfig* audioConfig);
	static AudioConfig* GetAudioConfigFromIndex(int index);
};