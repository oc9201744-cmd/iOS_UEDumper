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
        return GetExecutableInfo().address + 0xA34E980;
    }

    uintptr_t GetNamesPtr() const override
    {
        uintptr_t base = GetExecutableInfo().address;
        
        // gName() fonksiyonu çağrılıyor, resolved GNames adresi alınıyor
        uintptr_t resolved = reinterpret_cast<uintptr_t(*)(uintptr_t)>(
            base + 0x4BD8740
        )(base + 0xA1178B0);
        
        // Dumper bu adresten bir kez daha okuyacak (vm_rpm_ptr)
        // O yüzden resolved adresi statik bir yere yazıp pointer'ını döndürüyoruz
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
