#include "Config.h"
#include <filesystem>

#include "AudioConfigs.h"
#include "SirenConfigs.h"
#include "Mod.h"
#include "INIRead.h"
#include "Keybinds.h"

std::string Config::m_DataPath = "\\data\\";
std::string Config::m_DataFile = "data.json";

void Config::SaveJSON() {
	Log::file << "[Config] Saving config" << std::endl;

	CreatePath(m_DataPath);
	
	//DeleteAllConfig();

	Json::Value dataValue = Json::objectValue;

	dataValue["audioDistanceMult"] = Mod::m_AudioDistanceMult;
	dataValue["volume"] = Mod::m_Volume;

	dataValue["audios"] = Json::arrayValue;

	for (auto audioConfig : AudioConfigs::m_AudioConfigs) {
		auto value = audioConfig->ToJSON();

		dataValue["audios"].append(value);
	}

	dataValue["sirens"] = Json::arrayValue;

	for (auto sirenConfig : SirenConfigs::GetSirenConfigs()) {
		if (!sirenConfig->horn && sirenConfig->sirens.size() == 0) continue;

		auto value = sirenConfig->ToJSON();

		dataValue["sirens"].append(value);
	}

	WriteToFile(m_DataPath + m_DataFile, dataValue);
}

void Config::LoadJSON() {
	Log::file << "[Config] Loading config.ini" << std::endl;

	auto configIniPath = GetFullPath("config.ini");

	Keybinds::openMenu.SetKey(INIRead::GetString(configIniPath, "Key_Open_Menu", "key"));
	Keybinds::openMenu.ctrl = INIRead::GetBool(configIniPath, "Key_Open_Menu", "ctrl");
	Keybinds::openMenu.shift = INIRead::GetBool(configIniPath, "Key_Open_Menu", "shift");
	Keybinds::openMenu.alt = INIRead::GetBool(configIniPath, "Key_Open_Menu", "alt");

	Log::file << "[Config] Ley open menu: " << Keybinds::openMenu.GetKeybindString() << std::endl;

	Log::file << "[Config] Loading data" << std::endl;

	CreatePath(m_DataPath);

	if (!Exists(m_DataPath + m_DataFile)) return;

	auto value = ReadFile(m_DataPath + m_DataFile);

	Mod::m_AudioDistanceMult = value["audioDistanceMult"].asFloat();
	Mod::m_Volume = value["volume"].asFloat();

	for (size_t i = 0; i < value["audios"].size(); i++)
	{
		Json::Value audioValue = value["audios"][i];

		Log::file << "[Config] Loading audio '" << audioValue["fileName"].asString() << "'" << std::endl;

		AudioConfig* audioConfig = new AudioConfig("");
		audioConfig->FromJSON(audioValue);

		if (!Exists("audios\\" + audioConfig->fileName)) {
			Log::file << "[Config] File '" << audioConfig->fileName << "' not found" << std::endl;

			delete audioConfig;

			continue;
		}

		audioConfig->LoadSource();

		AudioConfigs::m_AudioConfigs.push_back(audioConfig);
	}

	for (size_t i = 0; i < value["sirens"].size(); i++)
	{
		Json::Value sirenValue = value["sirens"][i];

		Log::file << "[Config] Loading siren " << sirenValue["modelId"].asInt() << std::endl;

		SirenConfig* sirenConfig = new SirenConfig(0);
		sirenConfig->FromJSON(sirenValue);

		SirenConfigs::m_SirenConfigs[sirenConfig->modelId] = sirenConfig;

	}
}

void Config::DeleteAllConfig() {
	Log::file << "[Config] DeleteAllConfig" << std::endl;

	//std::filesystem::remove_all(m_DataPath + "settings.json");
}

std::string Config::GetFullPath(std::string path) {
	std::string pluginPath = PLUGIN_PATH("\\");
	std::string finalPath = pluginPath + path;
	return finalPath;
}

void Config::CreatePath(std::string path) {
	std::string pluginPath = PLUGIN_PATH("\\");
	std::string finalPath = pluginPath + path;

	if (!std::filesystem::is_directory(finalPath) || !std::filesystem::exists(finalPath)) {
		std::filesystem::create_directory(finalPath);
	}
}

bool Config::Exists(std::string path) {
	std::string pluginPath = PLUGIN_PATH("\\");
	std::string finalPath = pluginPath + path;
	return std::filesystem::exists(finalPath);
}

void Config::WriteToFile(std::string path, Json::Value value) {
	std::string pluginPath = PLUGIN_PATH("\\");
	std::string finalPath = pluginPath + path;

	//Log::file << "[Config] WriteToFile " << finalPath << std::endl;

	Json::StyledWriter writer;
	std::string strJson = writer.write(value);

	std::ofstream file(finalPath);
	file << strJson;
	file.close();
}

Json::Value Config::ReadFile(std::string path) {
	std::string pluginPath = PLUGIN_PATH("\\");
	std::string finalPath = pluginPath + path;

	//Log::file << "[Config] ReadFile " << finalPath << std::endl;

	std::ifstream file(finalPath);

	Json::Value value;
	Json::Reader reader;

	if (!reader.parse(file, value, true)) {
		MessageBox(HWND_DESKTOP, std::string("Error loading " + path).c_str(), "Vehicle Siren Lights", MB_ICONERROR);
	}

	return value;
}