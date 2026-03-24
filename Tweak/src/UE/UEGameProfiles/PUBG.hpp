#pragma once

#include "../UEGameProfile.hpp"
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

    // ===== GUObjectArray (FIXED) =====
    uintptr_t GetGUObjectArrayPtr() const override
    {
        std::string pattern = "80 B9 ? ? ? ? ? ? ? 91 ? ? 40 F9 ? 03 80 52";

        auto text = GetExecutableInfo().getSection("__TEXT", "__text");
        uintptr_t ins = KittyScanner::findIdaPatternFirst(text.start, text.end, pattern);
        if (!ins) return 0;

        uintptr_t adrp = Arm64::DecodeADRP(ins);
        uintptr_t add  = Arm64::DecodeADD(ins + 4);
        uintptr_t addr = adrp + add;

        if (!addr) return 0;

        return vm_rpm_ptr<uintptr_t>((void*)addr);
    }

    // ===== GNames (FIXED) =====
    uintptr_t GetNamesPtr() const override
    {
        std::string pattern = "ff c3 01 91 c0 03 5f d6 ? ? ? ? ? ? ? 91 ? ? ? 94 ? ? ? 34";

        auto text = GetExecutableInfo().getSection("__TEXT", "__text");
        uintptr_t ins = KittyScanner::findIdaPatternFirst(text.start, text.end, pattern);
        if (!ins) return 0;

        uintptr_t adrp = Arm64::DecodeADRP(ins + 0x1c);
        uintptr_t add  = Arm64::DecodeADD(ins + 0x1c + 4);
        uintptr_t addr = adrp + add;

        if (!addr) return 0;

        return vm_rpm_ptr<uintptr_t>((void*)addr);
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
