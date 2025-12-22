
#include "IntUtil.h"

std::vector<uint32_t> IntUtil::GetIntsFromJson(Json::Value json)
{
    std::vector<uint32_t> res;

    if (json.isInt())
    {
        auto biped = static_cast<uint32_t>(json.asInt());
        res.push_back(biped);
        return res;
    }

    if (json.isArray())
    {
        for (auto jsonIter : json)
        {
            if (!jsonIter.isInt())
            {
                continue;
            }

            auto biped = static_cast<uint32_t>(jsonIter.asInt());
            res.push_back(biped);
        }
    }

    return res;
}