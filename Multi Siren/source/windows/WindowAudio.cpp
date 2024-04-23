#include "WindowAudio.h"

#include "../AudioConfigs.h"

#include "../menu/Menu.h"
#include "../Vehicle.h"
#include "../Mod.h"

AudioConfig* WindowAudio::m_AudioConfig = NULL;

std::function<void(AudioConfig*)> WindowAudio::m_OnClose;
std::function<void(void)> WindowAudio::m_OnRemove;

void WindowAudio::Create() {
	auto window = Menu::AddWindow(Mod::m_Name, "Edit Audio");

	window->AddNumberRange("Volume", &m_AudioConfig->volume, 0.0f, 10.0f);

	auto removeAudio = window->AddButton("Remove");
	removeAudio->m_OnClick = [window]() {
		Menu::RemoveWindow(window);
		if (m_OnRemove) m_OnRemove();
		if (m_OnClose) m_OnClose(NULL);
	};

	auto buttonBack = window->AddButton("Back");
	buttonBack->m_OnClick = [window]() {
		Menu::RemoveWindow(window);
		if (m_OnClose) m_OnClose(NULL);
	};
}

void WindowAudio::CreateEmptyAudio() {
	auto window = Menu::AddWindow(Mod::m_Name, "Select Audio");
	window->m_MaxItems = 12;

	auto buttonBack = window->AddButton("[ Back ]");
	buttonBack->m_OnClick = [window]() {
		Menu::RemoveWindow(window);
		if (m_OnClose) m_OnClose(NULL);
	};

	std::string pluginPath = PLUGIN_PATH("\\");
	std::string path = pluginPath + "\\audios\\";

	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		std::string fileName = entry.path().filename().string();

		auto button = window->AddButton(fileName);
		button->m_OnClick = [window, fileName, entry]() {
			Menu::RemoveWindow(window);

			AudioConfig* audioConfig = NULL;

			for (auto ac : AudioConfigs::m_AudioConfigs)
			{
				if (ac->fileName.compare(fileName) == 0) audioConfig = ac;
			}

			if (audioConfig == NULL) {
				Log::file << "[WindowAudio] AudioConfig not found" << std::endl;

				audioConfig = AudioConfigs::AddAudioConfig(fileName);
			}
			else {
				Log::file << "[WindowAudio] AudioConfig already exists" << std::endl;
			}

			if (m_OnClose) m_OnClose(audioConfig);
		};
	}

	
}