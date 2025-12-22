#include "SteamWorkaround.h"

bool isLooksMenuOpen = false;
bool isConsoleOpen = false;

typedef void(WINAPI* SteamAPI_ReleaseCurrentThreadMemory)();

SteamAPI_ReleaseCurrentThreadMemory releaseMemory = NULL;

typedef void(WINAPI* SteamAPI_RunCallbacks)(void);

SteamAPI_RunCallbacks fpSteamAPI_RunCallbacks = NULL;

void DetourSteamAPI_RunCallbacks()
{
    if (isLooksMenuOpen || isConsoleOpen)
    {
        releaseMemory();

        return;
    }

    fpSteamAPI_RunCallbacks();
}

void SteamWorkaround::Hook()
{
    if (fpSteamAPI_RunCallbacks != NULL)
    {
        return;
    }

    auto handle = LoadLibraryA("steam_api64.dll");

    if (handle == NULL)
    {
        REX::INFO("Steam Api is NULL.");
        return;
    }

    releaseMemory = (SteamAPI_ReleaseCurrentThreadMemory)GetProcAddress(handle, "SteamAPI_ReleaseCurrentThreadMemory");

    if (releaseMemory == NULL)
    {
        REX::INFO("SteamAPI_ReleaseCurrentThreadMemory is NULL.");
        return;
    }

    if (MH_Initialize() != MH_OK)
    {
        REX::INFO("Could not initialize MinHook.");
        return;
    }

    if (MH_CreateHookApiEx(L"steam_api64.dll", "SteamAPI_RunCallbacks", &DetourSteamAPI_RunCallbacks,
        &fpSteamAPI_RunCallbacks) != MH_OK)
    {
        REX::INFO("Could not hook SteamAPI_RunCallbacks.");
        return;
    }

    if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
    {
        REX::INFO("Could not enable hooks.");
        return;
    }

    REX::INFO("MinHook initialization finished.");
}

void SteamWorkaround::SetMenu(std::string menu, bool isOpened)
{
    if (menu != "LooksMenu" && menu != "Console")
    {
        return;
    }

    if (menu == "LooksMenu")
    {
        isLooksMenuOpen = isOpened;
    }

    if (menu == "Console")
    {
        isConsoleOpen = isOpened;
    }

    REX::INFO(std::format("Event [{0}] is opening: {1}.", menu, isOpened));
}