#pragma once

#include "pch.h"

namespace SpellUtil
{
    bool HasSpell(RE::TESSpellList* form, RE::SpellItem* spell);

    void AddSpells(RE::TESSpellList* form, const std::vector<uint32_t>& spells);
}