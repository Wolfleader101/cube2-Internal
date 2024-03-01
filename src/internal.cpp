#include "internal.hpp"

void PatchInternal(BYTE* dst, BYTE* src, size_t size)
{
    DWORD oldProtect;
    VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldProtect);
    memcpy(dst, src, size);
    VirtualProtect(dst, size, oldProtect, &oldProtect);
}

uintptr_t FindDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets)
{
    uintptr_t addr = ptr;
    for (unsigned int i = 0; i < offsets.size(); ++i)
    {
        addr = *(uintptr_t*)addr;
        addr += offsets[i];
    }
    return addr;
}

NopInternal::NopInternal(BYTE* dst, size_t size) : dst(dst), size(size)
{
    originalCode = new BYTE[size];
    nopCode = new BYTE[size];

    memset(nopCode, 0x90, size);

    memcpy(originalCode, dst, size);
}

NopInternal::~NopInternal()
{
    delete[] originalCode;
    delete[] nopCode;
}

void NopInternal::Enable()
{
    PatchInternal(dst, nopCode, size);
}

void NopInternal::Disable()
{
    PatchInternal(dst, originalCode, size);
}

ManagedPatch::ManagedPatch(BYTE* dst, BYTE* src, size_t size)
    : dst(dst), newCode(new BYTE[size]), originalCode(new BYTE[size]), size(size)
{
    memcpy(newCode, src, size);
    memcpy(originalCode, dst, size);
}

ManagedPatch::~ManagedPatch()
{
    delete[] newCode;
    delete[] originalCode;
}

void ManagedPatch::Enable()
{
    PatchInternal(dst, newCode, size);
}

void ManagedPatch::Disable()
{
    PatchInternal(dst, originalCode, size);
}

Hook::Hook(void* toHook, void* ourFunct, int len) : toHook(toHook), oldOpcodes(nullptr), tLen(len), enabled(false)
{
    if (len < 5)
    {
        return;
    }

    DWORD curProtection;

    VirtualProtect(toHook, len, PAGE_EXECUTE_READWRITE, &curProtection);

    oldOpcodes = std::make_unique<BYTE[]>(len);

    if (oldOpcodes != nullptr)
    {
        for (int i = 0; i < len; i++)
        {
            oldOpcodes[i] = ((BYTE*)toHook)[i];
        }
    }

    memset(toHook, 0x90, len);

    DWORD relativeAddress = ((DWORD)ourFunct - (DWORD)toHook) - 5;

    *(BYTE*)toHook = 0xE9;

    *(DWORD*)((DWORD)toHook + 1) = relativeAddress; // addres of where to jump too

    VirtualProtect(toHook, len, curProtection, &curProtection);
}

Hook::~Hook()
{
    if (oldOpcodes != nullptr)
    {
        DWORD curProtection;

        VirtualProtect(toHook, tLen, PAGE_EXECUTE_READWRITE, &curProtection);

        for (int i = 0; i < tLen; i++)
        {
            ((BYTE*)toHook)[i] = oldOpcodes[i];
        }

        VirtualProtect(toHook, tLen, curProtection, &curProtection);
    }
}

void Hook::Enable()
{
    enabled = true;
}

void Hook::Disable()
{
    enabled = false;
}

bool Hook::IsEnabled()
{
    return enabled;
}