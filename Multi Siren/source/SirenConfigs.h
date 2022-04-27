#pragma once

#include "pch.h"

#include "SirenConfig.h"

class SirenConfigs {
public:
	static std::map<int, SirenConfig*> m_SirenConfigs;

	static SirenConfig* AddSirenConfig(int modelId);
	static bool ModelHasConfig(int modelId);
	static SirenConfig* GetSirenConfig(int modelId);
	static std::vector<SirenConfig*> GetSirenConfigs();
};