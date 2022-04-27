#include "pch.h"

class Mod {
public:
    static bool m_DebugEnabled;
    static std::string m_Version;
    static std::string m_Name;
    static ISoundEngine* m_SoundEngine;
    static float m_AudioDistanceMult;
    static float m_Volume;

    static void Init();
    static void Update();
    static void Draw();

    static float GetVolume();
    static void MuteSounds();

    static void ToggleMenu();
    static void SetPlayerControl(bool enabled);

    Mod();
};