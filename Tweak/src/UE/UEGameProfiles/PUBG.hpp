// src/UE/UEGameProfiles/PUBG.hpp
#pragma once

#include "../UEGameProfile.hpp"   // IGameProfile interface'i buradan geliyor
#include "KittyMemory.h"          // KittyMemory include et (gerekliyse yolu düzelt)
#include <string>
#include <vector>

using namespace UEMemory;  // Eğer namespace varsa

class PUBGProfile : public IGameProfile
{
public:
    PUBGProfile() = default;

    std::string GetAppName() const override {
        return "PUBG";
    }

    std::vector<std::string> GetAppIDs() const override {
        return {
            "com.tencent.ig",
            "com.rekoo.pubgm",
            "com.pubg.imobile",
            "com.pubg.krmobile",
            "com.vng.pubgmobile"
        };
    }

    bool isUsingCasePreservingName() const override { return false; }
    bool IsUsingFNamePool() const override { return false; }
    bool isUsingOutlineNumberName() const override { return false; }

    // ===== GUObjectArray =====
    uintptr_t GetGUObjectArrayPtr() const override {
        // KittyMemory ile ShadowTrackerExtra base'ini al
        KittyMemory::MemoryFileInfo exeInfo = KittyMemory::getLibraryInfo("ShadowTrackerExtra");
        if (!exeInfo.valid) {
            // fallback veya log
            return 0;
        }
        uintptr_t base = exeInfo.address;  // <-- address field'ı kullan (baseAddress değil)
        uintptr_t offset = 0x10A34E980;
        return base + offset;
    }

    // ===== GNames =====
    uintptr_t GetNamesPtr() const override {
        KittyMemory::MemoryFileInfo exeInfo = KittyMemory::getLibraryInfo("ShadowTrackerExtra");
        if (!exeInfo.valid) return 0;
        uintptr_t base = exeInfo.address;
        uintptr_t offset = 0x10a1178b0;
        return base + offset;
    }

    // ===== GWorld =====
    uintptr_t GetGWorldPtr() const override {
        KittyMemory::MemoryFileInfo exeInfo = KittyMemory::getLibraryInfo("ShadowTrackerExtra");
        if (!exeInfo.valid) return 0;
        uintptr_t base = exeInfo.address;
        uintptr_t offset = 0x10A566E00;
        return base + offset;
    }

    UE_Offsets* GetOffsets() const override {
        static UE_Offsets offsets = UE_DefaultOffsets::UE4_18_19(isUsingCasePreservingName());

        static bool once = false;
        if (!once) {
            once = true;
            offsets.FNameEntry.Index = sizeof(void*);
            offsets.FNameEntry.Name  = sizeof(void*) + sizeof(int32_t);
        }

        return &offsets;
    }
};