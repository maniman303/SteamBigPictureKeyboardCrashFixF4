#include "SteamWorkaround.h"
#include "utils/SteamUtils.h"

bool isEnabled = false;

ISteamUtils* steamUtils = NULL;

typedef ISteamUtils*(__stdcall* GetISteamUtils)(void);

typedef void(WINAPI* SteamAPI_ReleaseCurrentThreadMemory)();

SteamAPI_ReleaseCurrentThreadMemory releaseMemory = NULL;

typedef void(WINAPI* SteamAPI_RunCallbacks)(void);

SteamAPI_RunCallbacks fpSteamAPI_RunCallbacks = NULL;

void DetourSteamAPI_RunCallbacks()
{
    if (isEnabled)
    {
        fpSteamAPI_RunCallbacks();
        return;
    }

    releaseMemory();
}

using ShowFloatingGamepadTextInputFn = bool(__thiscall*)(ISteamUtils* thisptr, EFloatingGamepadTextInputMode eKeyboardMode, int nTextFieldXPosition, int nTextFieldYPosition, int nTextFieldWidth, int nTextFieldHeight);

ShowFloatingGamepadTextInputFn originalShowFloatingGamepadTextInput;

bool DetourShowFloatingGamepadTextInput(ISteamUtils* thisptr, EFloatingGamepadTextInputMode eKeyboardMode, int nTextFieldXPosition, int nTextFieldYPosition, int nTextFieldWidth, int nTextFieldHeight)
{
    REX::INFO("Showing floating gamepad text input.");

    isEnabled = true;

    return originalShowFloatingGamepadTextInput(thisptr, eKeyboardMode, nTextFieldXPosition, nTextFieldYPosition, nTextFieldWidth, nTextFieldHeight);
}

void HookShowFloatingGamepadTextInput(ISteamUtils* utils)
{
    void** vtable = *(void***)utils;

    int index = 35;

    DWORD oldProtect;
    VirtualProtect(
        &vtable[index],
        sizeof(void*),
        PAGE_EXECUTE_READWRITE,
        &oldProtect
    );

    originalShowFloatingGamepadTextInput = (ShowFloatingGamepadTextInputFn)vtable[index];

    vtable[index] = (void*)&DetourShowFloatingGamepadTextInput;

    VirtualProtect(
        &vtable[index],
        sizeof(void*),
        oldProtect,
        &oldProtect
    );

    REX::INFO("Hooked ShowFloatingGamepadTextInput.");
}

using ShowGamepadTextInputFn = bool(__thiscall*)(ISteamUtils* thisptr, EGamepadTextInputMode eInputMode, EGamepadTextInputLineMode eLineInputMode, const char *pchDescription, uint32_t unCharMax, const char *pchExistingText);

ShowGamepadTextInputFn originalShowGamepadTextInput;

bool DetourShowGamepadTextInput(ISteamUtils* thisptr, EGamepadTextInputMode eInputMode, EGamepadTextInputLineMode eLineInputMode, const char *pchDescription, uint32_t unCharMax, const char *pchExistingText)
{
    REX::INFO("Showing gamepad text input.");

    isEnabled = true;

    return originalShowGamepadTextInput(thisptr, eInputMode, eLineInputMode, pchDescription, unCharMax, pchExistingText);
}

void HookShowGamepadTextInput(ISteamUtils* utils)
{
    void** vtable = *(void***)utils;

    int index = 20;

    DWORD oldProtect;
    VirtualProtect(
        &vtable[index],
        sizeof(void*),
        PAGE_EXECUTE_READWRITE,
        &oldProtect
    );

    originalShowGamepadTextInput = (ShowGamepadTextInputFn)vtable[index];

    vtable[index] = (void*)&DetourShowGamepadTextInput;

    VirtualProtect(
        &vtable[index],
        sizeof(void*),
        oldProtect,
        &oldProtect
    );

    REX::INFO("Hooked ShowGamepadTextInput.");
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

    auto steamUtilsConstructor = (GetISteamUtils)GetProcAddress(handle, "SteamAPI_SteamUtils_v010");

    if (steamUtilsConstructor == NULL)
    {
        REX::INFO("GetISteamUtils is NULL.");
        return;
    }

    steamUtils = steamUtilsConstructor();

    if (steamUtils == NULL)
    {
        REX::INFO("SteamUtils not found.");
        return;
    }

    HookShowFloatingGamepadTextInput(steamUtils);

    HookShowGamepadTextInput(steamUtils);

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