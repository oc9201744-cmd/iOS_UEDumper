#pragma once

#include "../UEGameProfile.hpp"
#include <cstring>

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

    // ===== SADECE DOĞRU OLANI KULLAN =====
    static constexpr uintptr_t GWORLD = 0x10A566E00;
    static constexpr uintptr_t NAMES  = 0x10A1178B0;

    // ❌ GUObjectArray devre dışı
    uintptr_t GetGUObjectArrayPtr() const override
    {
        return 0;
    }

    uintptr_t GetNamesPtr() const override
    {
        return NAMES;
    }

    uintptr_t GetGWorldPtr() const
    {
        return vm_rpm_ptr<uintptr_t>((void*)GWORLD);
    }

    UE_Offsets *GetOffsets() const override
    {
        static UE_Offsets offsets;

        static bool once = false;
        if (!once)
        {
            once = true;

            offsets.UObject.ObjectFlags = 0x8;
            offsets.UObject.InternalIndex = 0xC;
            offsets.UObject.ClassPrivate = 0x10;
            offsets.UObject.NamePrivate = 0x18;
            offsets.UObject.OuterPrivate = 0x20;

            offsets.FNameEntry.Index = sizeof(void *);
            offsets.FNameEntry.Name  = sizeof(void *) + sizeof(int32_t);

            offsets.UStruct.SuperStruct = 0x30;
            offsets.UStruct.Children = 0x38;
            offsets.UStruct.PropertiesSize = 0x40;

            offsets.UFunction.Func = 0xB0;
            offsets.UFunction.NumParams = 0x8C;
            offsets.UFunction.ParamSize = 0x8E;

            offsets.UProperty.ArrayDim = 0x30;
            offsets.UProperty.ElementSize = 0x34;
            offsets.UProperty.PropertyFlags = 0x38;
            offsets.UProperty.Offset_Internal = 0x44;
        }

        return &offsets;
    }
};
