// src/UE/UEGameProfiles/PUBG.hpp (güncellenmiş kısım)
    // ===== GUObjectArray =====
    uintptr_t GetGUObjectArrayPtr() const override
    {
        auto exeInfo = GetExecutableInfo();  // KittyMemory::MemoryFileInfo döner varsayalım
        uintptr_t base = exeInfo.address;    // <-- BURAYI DEĞİŞTİR: baseAddress → address
        uintptr_t offset = 0x10A34E980;
        return base + offset;
    }

    // ===== GNames =====
    uintptr_t GetNamesPtr() const override
    {
        auto exeInfo = GetExecutableInfo();
        uintptr_t base = exeInfo.address;    // <-- aynı değişiklik
        uintptr_t offset = 0x10a1178b0;
        return base + offset;
    }

    // ===== GWorld =====
    uintptr_t GetGWorldPtr() const override
    {
        auto exeInfo = GetExecutableInfo();
        uintptr_t base = exeInfo.address;    // <-- aynı
        uintptr_t gworld_data_offset = 0x10A566E00;
        return base + gworld_data_offset;
    }
