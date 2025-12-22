#pragma once

#include "pch.h"
#include "MinhookExtension.h"

namespace SteamWorkaround
{
    void Hook();

    void SetMenu(std::string menu, bool isOpened);
}