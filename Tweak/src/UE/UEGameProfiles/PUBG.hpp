#pragma once

#include "../UEGameProfile.hpp"
using namespace UEMemory;

class PUBGProfile : public IGameProfile
{
public:
    PUBGProfile() = default;

    std::string GetAppName() const override
    {
        return "PUBG";
    }

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

    bool isUsingCasePreservingName() const override
    {
        return false;
    }

    bool IsUsingFNamePool() const override
    {
        return false;
    }

    bool isUsingOutlineNumberName() const override
    {
        return false;
    }

    uintptr_t GetGUObjectArrayPtr() const override
    {
        KittyMemory::MemoryFileInfo lib = KittyMemory::getLibraryInfo("ShadowTrackerExtra");
        if (lib.valid && lib.address != 0)
        {
            return lib.address + 0x10A34E980;
        }
        return 0;
    }

    uintptr_t GetNamesPtr() const override
    {
        KittyMemory::MemoryFileInfo lib = KittyMemory::getLibraryInfo("ShadowTrackerExtra");
        if (lib.valid && lib.address != 0)
        {
            return lib.address + 0x10a1178b0;
        }
        return 0;
    }

    UE_Offsets *GetOffsets() const override
    {
        static UE_Offsets offsets = UE_DefaultOffsets::UE4_18_19(isUsingCasePreservingName());

        static bool once = false;
        if (!once)
        {
            once = true;
            offsets.FNameEntry.Index = sizeof(void *);
            offsets.FNameEntry.Name = sizeof(void *) + sizeof(int32_t);
        }

        return &offsets;
    }
};