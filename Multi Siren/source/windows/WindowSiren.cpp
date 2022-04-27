#include "WindowSiren.h"
#include "WindowMain.h"
#include "WindowAudio.h"

#include "../AudioConfigs.h"

#include "../menu/Menu.h"
#include "../Vehicle.h"
#include "../Mod.h"

#include "../Vehicles.h"

SirenConfig* WindowSiren::m_SirenConfig = NULL;

void WindowSiren::Create() {
	int modelId = WindowMain::m_Vehicle->m_nModelIndex;

	auto window = Menu::AddWindow(Mod::m_Name, "Siren");

	if (!SirenConfigs::ModelHasConfig(modelId)) {
		m_SirenConfig = SirenConfigs::AddSirenConfig(modelId);
	}
	m_SirenConfig = SirenConfigs::GetSirenConfig(modelId);


	window->AddItem("---------- " + std::to_string(m_SirenConfig->sirens.size()) + " Sirens ----------");

	auto buttonAddAudio = window->AddButton("Add");
	buttonAddAudio->m_OnClick = [window]() {
		Menu::RemoveWindow(window);

		WindowAudio::m_OnClose = [](AudioConfig* audioConfig) {
			if (audioConfig) {
				m_SirenConfig->AddSirenAudio(audioConfig);
			}
			WindowSiren::Create();
		};
		WindowAudio::CreateEmptyAudio();
	};

	for (auto audioConfig : m_SirenConfig->sirens)
	{
		auto button = window->AddButton("Edit audio (" + audioConfig->fileName + ")");
		button->m_OnClick = [window, audioConfig]() {
			Menu::RemoveWindow(window);

			WindowAudio::m_OnClose = [](AudioConfig*) {
				WindowSiren::Create();
			};
			WindowAudio::m_OnRemove = [audioConfig]() {
				Vehicles::RemoveAllVehicles();

				m_SirenConfig->RemoveSirenAudio(audioConfig);

				Vehicles::TryAddAllVehicles();
			};
			WindowAudio::m_AudioConfig = audioConfig;
			WindowAudio::Create();
		};
	}

	

	window->AddItem("--------------------------------------");

	auto button1 = window->AddButton("Edit horn" + (m_SirenConfig->horn ? (" (" + m_SirenConfig->horn->fileName + ")") : ""));
	button1->m_OnClick = [window]() {
		Menu::RemoveWindow(window);

		if (!m_SirenConfig->horn) {
			WindowAudio::m_OnClose = [](AudioConfig* audioConfig) {
				if (audioConfig) {
					m_SirenConfig->SetHorn(audioConfig);
				}
				WindowSiren::Create();
			};
			WindowAudio::CreateEmptyAudio();
		}
		else {
			WindowAudio::m_OnClose = [](AudioConfig* audioConfig) {
				WindowSiren::Create();
			};
			WindowAudio::m_OnRemove = []() {
				Vehicles::RemoveAllVehicles();

				m_SirenConfig->RemoveHorn();

				Vehicles::TryAddAllVehicles();
			};
			WindowAudio::m_AudioConfig = m_SirenConfig->horn;
			WindowAudio::Create();
		}
	};
}