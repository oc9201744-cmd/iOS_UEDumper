// ===== GUObjectArray =====
uintptr_t GetGUObjectArrayPtr() const override
{
    uintptr_t base = GetExecutableInfo().baseAddress;  // ShadowTrackerExtra base (ASLR slide dahil)
    uintptr_t offset = 0x10A34E980;
    return base + offset;
    // veya vm_rpm_ptr<uintptr_t>((void*)(base + offset)); eğer ekstra read lazım
}

// ===== GNames =====
uintptr_t GetNamesPtr() const override
{
    uintptr_t base = GetExecutableInfo().baseAddress;
    uintptr_t offset = 0x10a1178b0;  // GNameData
    return base + offset;
    // Eğer GNameFunc lazım olursa:
    // uintptr_t gname_func = base + 0x104bd8740;
    // Ama genelde GNameData direkt pointer yeterli
}

// Ekstra: GWorld için (eğer ihtiyacın olursa)
uintptr_t GetGWorldPtr() const override
{
    uintptr_t base = GetExecutableInfo().baseAddress;
    uintptr_t gworld_func_offset = 0x102A62208;
    uintptr_t gworld_data_offset = 0x10A566E00;

    // Genelde GWorldFunction çağrılıp data okunur, veya direkt data pointer
    return base + gworld_data_offset;
}
