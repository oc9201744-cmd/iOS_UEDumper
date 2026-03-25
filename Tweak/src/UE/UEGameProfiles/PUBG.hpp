#pragma once

#include "../UEGameProfile.hpp"
#include <mach-o/dyld.h>
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
        uintptr_t slide = (uintptr_t)_dyld_get_image_vmaddr_slide(0);
        return 0x10A34E980 + slide;
    }

    uintptr_t GetNamesPtr() const override
    {
        uintptr_t slide = (uintptr_t)_dyld_get_image_vmaddr_slide(0);

        uintptr_t func = 0x104BD8740 + slide;
        uintptr_t data = 0x10A1178B0 + slide;

        uintptr_t resolved = reinterpret_cast<uintptr_t(*)(uintptr_t)>(func)(data);

        static uintptr_t gNamesResolved = 0;
        gNamesResolved = resolved;
        return reinterpret_cast<uintptr_t>(&gNamesResolved);
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
