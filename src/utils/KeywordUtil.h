#pragma once

#include "pch.h"
#include <unordered_set>

namespace KeywordUtil
{
    std::unordered_set<uint32_t> GetKeywordsIds(RE::BGSKeywordForm* form);

    void AddKeywords(RE::BGSKeywordForm* form, const std::vector<uint32_t>& keywords);
}