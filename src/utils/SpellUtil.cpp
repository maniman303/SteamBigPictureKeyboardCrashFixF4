#include "SpellUtil.h"
#include "FormUtil.h"

bool SpellUtil::HasSpell(RE::TESSpellList* form, RE::SpellItem* spell)
{
    if (form == NULL || spell == NULL)
    {
        return true;
    }

    auto spellList = form->spellData;
    if (spellList == NULL)
    {
        return true;
    }

    for (uint32_t i = 0; i < spellList->numSpells; i++)
    {
        auto iter = spellList->spells[i];

        if (iter == spell)
        {
            return true;
        }
    }

    return false;
}

void SpellUtil::AddSpells(RE::TESSpellList* form, const std::vector<uint32_t>& spells)
{
    if (form == NULL)
    {
        return;
    }

    for (auto spellId : spells)
    {
        auto spell = FormUtil::GetFormAs<RE::SpellItem>(spellId);
        if (spell == NULL)
        {
            continue;
        }

        if (!SpellUtil::HasSpell(form, spell))
        {
            form->AddSpell(spell);
        }
    }
}