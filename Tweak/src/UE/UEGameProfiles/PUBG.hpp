// src/UE/UEGameProfiles/PUBG.hpp
#pragma once

#include "../UEGameProfile.hpp"  // IGameProfile ve UEMemory include'ları buradan gelsin
#include <string>
#include <vector>

using namespace UEMemory;

class PUBGProfile : public IGameProfile
{
public:
    PUBGProfile() = default;

    std::string GetAppName() const override { return "PUBG"; }

    std::vector<std::string> GetAppIDs() const override
    {
        return {
            "com.tencent.ig",
            "com.rekoo.pubgm",
            "com.pubg.imobile",
            "com.pubg.krmobile",
            "com.vng.pubgmobile",
        };
    }

    bool isUsingCasePreservingName() const override { return false; }
    bool IsUsingFNamePool() const override { return false; }
    bool isUsingOutlineNumberName() const override { return false; }

    // ===== GUObjectArray =====
    uintptr_t GetGUObjectArrayPtr() const override
    {
        uintptr_t base = GetExecutableInfo().baseAddress;  // Bu fonksiyon UEMemory'den gelmeli
        uintptr_t offset = 0x10A34E980;
        return base + offset;
    }

    // ===== GNames =====
    uintptr_t GetNamesPtr() const override
    {
        uintptr_t base = GetExecutableInfo().baseAddress;
        uintptr_t offset = 0x10a1178b0;  // GNameData
        return base + offset;
    }

    // ===== GWorld (ekstra) =====
    uintptr_t GetGWorldPtr() const override
    {
        uintptr_t base = GetExecutableInfo().baseAddress;
        uintptr_t gworld_data_offset = 0x10A566E00;
        return base + gworld_data_offset;
        // Veya GWorldFunc'u kullanmak istersen: base + 0x102A62208
    }

    UE_Offsets *GetOffsets() const override
    {
        static UE_Offsets offsets = UE_DefaultOffsets::UE4_18_19(isUsingCasePreservingName());

        static bool once = false;
        if (!once)
        {
            once = true;
            offsets.FNameEntry.Index = sizeof(void *);
            offsets.FNameEntry.Name  = sizeof(void *) + sizeof(int32_t);
        }

        return &offsets;
    }
};
