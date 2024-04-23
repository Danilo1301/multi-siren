#include "WindowMain.h"

#include "WindowSiren.h"

#include "../menu/Menu.h"
#include "../Vehicle.h"
#include "../Mod.h"

CVehicle* WindowMain::m_Vehicle = nullptr;

void WindowMain::Create() {
	auto window = Menu::AddWindow(Mod::m_Name, "Vehicle ID " + std::to_string(m_Vehicle->m_nModelIndex) + " - v" + Mod::m_Version);

	auto buttonAudios = window->AddButton("Edit Sirens");
	buttonAudios->m_OnClick = [window]() {
		Menu::RemoveWindow(window);
		WindowSiren::Create();
	};

	auto audioDistanceMult = window->AddNumberRange("AudioDistanceMultiplier", &Mod::m_AudioDistanceMult, -10.0f, 10.0f);
	audioDistanceMult->m_HoldToChange = true;

	auto volume = window->AddNumberRange("Volume", &Mod::m_Volume, 0.0f, 1.0f);
	volume->m_HoldToChange = true;
}