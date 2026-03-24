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
    bool IsUsingFNamePool() const override { return true; } // düzeltildi
    bool isUsingOutlineNumberName() const override { return false; }

    // ===== DOĞRUDAN RUNTIME ADRESLER (BASE EKLEME YOK) =====
    static constexpr uintptr_t GUOBJECTARRAY = 0x10A34E980;
    static constexpr uintptr_t NAMES         = 0x10F63ACA0;
    static constexpr uintptr_t GENGINE       = 0x10A565BF0;

    uintptr_t GetGUObjectArrayPtr() const override
    {
        return GUOBJECTARRAY;
    }

    uintptr_t GetNamesPtr() const override
    {
        return NAMES;
    }

    uintptr_t GetGEnginePtr() const
    {
        return vm_rpm_ptr<uintptr_t>((void*)GENGINE);
    }

    uintptr_t GetGWorldPtr() const
    {
        uintptr_t GEngine = GetGEnginePtr();
        if (!GEngine) return 0;

        uintptr_t GameViewport =
            vm_rpm_ptr<uintptr_t>((void*)(GEngine + 0x30));
        if (!GameViewport) return 0;

        return vm_rpm_ptr<uintptr_t>((void*)(GameViewport + 0x30));
    }

    UE_Offsets *GetOffsets() const override
    {
        static UE_Offsets offsets;

        static bool once = false;
        if (!once)
        {
            once = true;

            // UObject
            offsets.UObject.ObjectFlags = 0x8;
            offsets.UObject.InternalIndex = 0xC;
            offsets.UObject.ClassPrivate = 0x10;
            offsets.UObject.NamePrivate = 0x18;
            offsets.UObject.OuterPrivate = 0x20;

            // FName (pool)
            offsets.FNameEntry.Index = 0;
            offsets.FNameEntry.Name  = 0x4;

            // Struct
            offsets.UStruct.SuperStruct = 0x30;
            offsets.UStruct.Children = 0x38;
            offsets.UStruct.PropertiesSize = 0x40;

            // Function
            offsets.UFunction.Func = 0xB0;
            offsets.UFunction.NumParams = 0x8C;
            offsets.UFunction.ParamSize = 0x8E;

            // Property
            offsets.UProperty.ArrayDim = 0x30;
            offsets.UProperty.ElementSize = 0x34;
            offsets.UProperty.PropertyFlags = 0x38;
            offsets.UProperty.Offset_Internal = 0x44;
        }

        return &offsets;
    }
};
