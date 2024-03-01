#pragma once

#include <Windows.h>

#include <TlHelp32.h>

#include <memory>
#include <vector>

inline uintptr_t GetModuleBaseAddress(const wchar_t* modName)
{
    return (uintptr_t)GetModuleHandleW(modName);
}

inline uintptr_t GetModuleBaseAddress(const char* modName)
{
    return (uintptr_t)GetModuleHandleA(modName);
}

void PatchInternal(BYTE* dst, BYTE* src, size_t size);

uintptr_t FindDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets);

class NopInternal
{
  public:
    NopInternal(BYTE* dst, size_t size);
    ~NopInternal();

    void Enable();
    void Disable();

  private:
    BYTE* dst;
    size_t size;
    BYTE* originalCode;
    BYTE* nopCode;
};

class ManagedPatch
{
  public:
    ManagedPatch(BYTE* dst, BYTE* src, size_t size);
    ~ManagedPatch();

    void Enable();
    void Disable();

  private:
    BYTE* dst;
    BYTE* newCode;
    size_t size;
    BYTE* originalCode;
};

class Hook
{
  public:
    Hook(void* toHook, void* ourFunct, int len);
    ~Hook();

    void Enable();
    void Disable();
    bool IsEnabled();

  private:
    void* toHook;
    std::unique_ptr<BYTE[]> oldOpcodes;
    int tLen;
    bool enabled;
};

class TrampHook
{
  public:
    TrampHook(void* toHook, void* ourFunct, int len);
    ~TrampHook();

    void Enable();
    void Disable();
    bool IsEnabled();
    void* GetGateway();

  private:
    void* gateway;
    std::unique_ptr<Hook> managedHook;
};
