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
    if (len < 16)
    {
        return;
    }

    DWORD curProtection;

    VirtualProtect(toHook, len, PAGE_EXECUTE_READWRITE, &curProtection);
    oldOpcodes = std::make_unique<BYTE[]>(len);
    memcpy(oldOpcodes.get(), toHook, len);

    memset(toHook, 0x90, len);

    // 64-bit absolute jump sequence
    BYTE jmpSeq[] = {
        0x50,                                           // push rax
        0x48, 0xB8,                                     // mov rax, ourFunct (placeholder for next 8 bytes)
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ourFunct address (placeholder)
        0xFF, 0xE0,                                     // jmp rax
        0x58                                            // pop rax
    };
    *(uintptr_t*)&jmpSeq[3] = (uintptr_t)ourFunct;

    memcpy(toHook, jmpSeq, sizeof(jmpSeq));
    VirtualProtect(toHook, len, curProtection, &curProtection);
}

Hook::~Hook()
{
    if (oldOpcodes != nullptr)
    {
        DWORD curProtection;

        VirtualProtect(toHook, tLen, PAGE_EXECUTE_READWRITE, &curProtection);

        memcpy(toHook, oldOpcodes.get(), tLen);
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

TrampHook::TrampHook(void* toHook, void* ourFunct, int len) : gateway(nullptr), managedHook(nullptr)
{
    if (len < 16)
        return; // we need 16 bytes to make a jump in x64

    gateway = VirtualAlloc(0, len + 16, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

    memcpy_s(gateway, len, toHook, len);

    BYTE* gatewayEnd = (BYTE*)gateway + len + 1;

    // Absolute jump back to the original function (after the overwritten bytes)
    BYTE jmpBack[] = {
        0x50,                                           // push rax
        0x48, 0xB8,                                     // mov rax, addr (placeholder for next 8 bytes)
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // address to jump back to
        0xFF, 0xE0,                                     // jmp rax
        0x58                                            // pop rax
    };
    uintptr_t addrToJumpBack = (uintptr_t)toHook + len;
    *(uintptr_t*)&jmpBack[3] = addrToJumpBack;

    memcpy(gatewayEnd, jmpBack, sizeof(jmpBack));

    managedHook = std::make_unique<Hook>(toHook, gateway, len);
}

TrampHook::~TrampHook()
{
    if (managedHook != nullptr)
    {
        managedHook->Disable();
        managedHook.reset();
    }
    if (gateway != nullptr)
    {
        VirtualFree(gateway, 0, MEM_RELEASE);
    }
}

void TrampHook::Enable()
{
    managedHook->Enable();
}

void TrampHook::Disable()
{
    managedHook->Disable();
}

bool TrampHook::IsEnabled()
{
    return managedHook->IsEnabled();
}

void* TrampHook::GetGateway()
{
    return gateway;
}
