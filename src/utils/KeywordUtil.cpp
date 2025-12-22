#include "KeywordUtil.h"
#include "FormUtil.h"

std::unordered_set<uint32_t> KeywordUtil::GetKeywordsIds(RE::BGSKeywordForm* form)
{
    std::unordered_set<uint32_t> res;

    if (form == NULL || form->keywords == NULL)
    {
        return res;
    }

    res.reserve(form->numKeywords);

    for (uint32_t i = 0; i < form->numKeywords; i++)
    {
        auto keyword = form->keywords[i];
        if (keyword == NULL)
        {
            continue;
        }

        res.insert(keyword->GetFormID());
    }

    return res;
}

void KeywordUtil::AddKeywords(RE::BGSKeywordForm* form, const std::vector<uint32_t>& keywords)
{
    if (form == NULL)
    {
        return;
    }

    for (auto keywordId : keywords)
    {
        auto keyword = FormUtil::GetFormAs<RE::BGSKeyword>(keywordId);

        if (keyword != NULL && !form->HasKeyword(keyword))
        {
            form->AddKeyword(keyword);
        }
    }
}