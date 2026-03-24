#pragma once

#include "../UEGameProfile.hpp"
#include <mach-o/dyld.h>
#include <cstring>

using namespace UEMemory;

class PUBGProfile : public IGameProfile
{
private:
    // ===== BASE (slide + preferred) =====
    static uintptr_t GetImageBase()
    {
        static uintptr_t base = 0;
        if (!base)
        {
            const struct mach_header_64* header =
                (const struct mach_header_64*)_dyld_get_image_header(0);

            uintptr_t slide = _dyld_get_image_vmaddr_slide(0);
            base = (uintptr_t)header + slide;
        }
        return base;
    }

    // ===== NORMALIZE (ABS → RELATIVE) =====
    static constexpr uintptr_t NORMALIZE(uintptr_t addr)
    {
        return addr - 0x100000000;
    }

public:
    // ===== RAW (LOG) =====
    static constexpr uintptr_t GUOBJECTARRAY = 0x10A34E980;
    static constexpr uintptr_t NAMES         = 0x10F63ACA0;
    static constexpr uintptr_t GENGINE       = 0x10A565BF0;

    // ===== FINAL RUNTIME =====
    uintptr_t GetGUObjectArrayPtr() const override
    {
        return GetImageBase() + NORMALIZE(GUOBJECTARRAY);
    }

    uintptr_t GetNamesPtr() const override
    {
        return GetImageBase() + NORMALIZE(NAMES);
    }

    uintptr_t GetGEnginePtr() const
    {
        uintptr_t addr = GetImageBase() + NORMALIZE(GENGINE);
        return vm_rpm_ptr<uintptr_t>((void*)addr);
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

    // ===== FLAGS =====
    bool isUsingCasePreservingName() const override { return false; }
    bool IsUsingFNamePool() const override { return false; }
    bool isUsingOutlineNumberName() const override { return false; }

    // ===== OFFSETS =====
    UE_Offsets *GetOffsets() const override
    {
        static UE_Offsets offsets;

        static bool once = false;
        if (!once)
        {
            once = true;

            offsets.Object.ObjectFlags = 0x8;
            offsets.Object.InternalIndex = 0xC;
            offsets.Object.ClassPrivate = 0x10;
            offsets.Object.NamePrivate = 0x18;
            offsets.Object.OuterPrivate = 0x20;

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
