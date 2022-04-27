#include "SirenConfigs.h"

std::map<int, SirenConfig*> SirenConfigs::m_SirenConfigs;

SirenConfig* SirenConfigs::AddSirenConfig(int modelId) {
	SirenConfig* sirenConfig = new SirenConfig(modelId);
	m_SirenConfigs[modelId] = sirenConfig;
	return sirenConfig;
}


bool SirenConfigs::ModelHasConfig(int modelId) {
	return m_SirenConfigs[modelId];
}

SirenConfig* SirenConfigs::GetSirenConfig(int modelId) {
	return m_SirenConfigs[modelId];
}

std::vector<SirenConfig*> SirenConfigs::GetSirenConfigs() {
	std::vector<SirenConfig*> vec;
	for (auto pair : SirenConfigs::m_SirenConfigs) {
		auto sirenConfig = pair.second;
		if (!sirenConfig) continue;
		vec.push_back(sirenConfig);
	}
	return vec;
}