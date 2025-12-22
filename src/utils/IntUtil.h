#pragma once

#include <json/json.h>
#include "pch.h"

namespace IntUtil
{
    std::vector<uint32_t> GetIntsFromJson(Json::Value json);
}