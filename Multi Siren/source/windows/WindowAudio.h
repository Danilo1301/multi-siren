#pragma once

#include "../pch.h"

#include "../SirenConfigs.h"

class WindowAudio {
public:
	static AudioConfig* m_AudioConfig;

	static std::function<void(AudioConfig*)> m_OnClose;
	static std::function<void(void)> m_OnRemove;

	static void Create();
	static void CreateEmptyAudio();
};