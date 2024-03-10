#include <Windows.h>
#include <chrono>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <thread>

#define _USE_MATH_DEFINES
#include <math.h>

#include <tchar.h>

#include "Math.hpp"
#include "glDraw.hpp"
#include "internal.hpp"
#include "player.h"

std::mutex hookMtx;

typedef BOOL(__stdcall* twglSwapBuffers)(HDC hDc);

constexpr int SELECT_CHAINSAW = WM_APP + 100;
constexpr int SELECT_SHOTGUN = WM_APP + 101;
constexpr int SELECT_MACHINEGUN = WM_APP + 102;
constexpr int SELECT_ROCKETLAUNCHER = WM_APP + 103;
constexpr int SELECT_RIFLE = WM_APP + 104;
constexpr int SELECT_GRENADELAUNCHER = WM_APP + 105;
constexpr int SELECT_PISTOL = WM_APP + 106;
constexpr int SELECT_FIREBALL = WM_APP + 107;
constexpr int SELECT_ICEBALL = WM_APP + 108;
constexpr int SELECT_LASER = WM_APP + 109;
constexpr int SELECT_BITE = WM_APP + 110;
constexpr int SELECT_NOSOUND = WM_APP + 111;

constexpr int FREEZE_AMMO = WM_APP + 112;
constexpr int GOD_MODE = WM_APP + 113;
constexpr int RAPID_FIRE = WM_APP + 114;

constexpr int UPDATE = WM_APP + 115;

constexpr int EDIT_CHAINSAW_AMMO = WM_APP + 116;
constexpr int EDIT_SHOTGUN_AMMO = WM_APP + 117;
constexpr int EDIT_MACHINEGUN_AMMO = WM_APP + 118;
constexpr int EDIT_ROCKETLAUNCHER_AMMO = WM_APP + 119;
constexpr int EDIT_RIFLE_AMMO = WM_APP + 120;
constexpr int EDIT_GRENADELAUNCHER_AMMO = WM_APP + 121;
constexpr int EDIT_PISTOL_AMMO = WM_APP + 122;
constexpr int EDIT_FIREBALL_AMMO = WM_APP + 123;
constexpr int EDIT_ICEBALL_AMMO = WM_APP + 124;
constexpr int EDIT_LASER_AMMO = WM_APP + 125;
constexpr int EDIT_BITE_AMMO = WM_APP + 126;
constexpr int EDIT_NOSOUND_AMMO = WM_APP + 127;

constexpr int TRIGGER_BOT = WM_APP + 128;
constexpr int NO_RECOIL = WM_APP + 129;
constexpr int AIMBOT = WM_APP + 130;

static Player* player = nullptr;
static TraceLine_t TraceLine = nullptr;
static Vec3* worldpos = nullptr;

static Player** entityList = nullptr;
static int* entityCount = nullptr;

static int tempSize = 0;

static Vec2 screenPos;
static Vec3 tmpWorldPos;
static Vec3 playerPos;
static float* viewMatrix;

static std::shared_ptr<NopInternal> freezeAmmo = nullptr;
static std::shared_ptr<NopInternal> godMode = nullptr;
static std::shared_ptr<NopInternal> rapidFire = nullptr;
static std::shared_ptr<ManagedPatch> noRecoilPatch = nullptr;
static std::shared_ptr<TrampHook> espHook = nullptr;

static std::map<int, HGLRC> contexts;

static HWND hEditChainsawAmmo;
static HWND hEditShotgunAmmo;
static HWND hEditMachinegunAmmo;
static HWND hEditRocketlauncherAmmo;
static HWND hEditRifleAmmo;
static HWND hEditGrenadelauncherAmmo;
static HWND hEditPistolAmmo;
static HWND hEditFireballAmmo;
static HWND hEditIceballAmmo;
static HWND hEditLaserAmmo;
static HWND hEditBiteAmmo;
static HWND hEditNosoundAmmo;

static HWND hFreezeAmmoBtn;
static HWND hGodModeBtn;
static HWND hRapidFireBtn;
static HWND hTriggerBotBtn;
static HWND hNoRecoilBtn;
static HWND hAimbotBtn;

static TCHAR buffer[16];

static bool isAmmoFrozen = false;
static bool isGodModeEnabled = false;
static bool isRapidFireEnabled = false;
static bool isInitialized = false;
static bool isTriggerBotEnabled = false;
static bool isNoRecoilEnabled = false;
static bool isAimbotEnabled = false;

static void selectWeapon(int weapon)
{
    player->selectedGun = weapon;
}

static void updateAmmoValues()
{
    SetWindowText(hEditChainsawAmmo, std::to_wstring(player->chainSaw).c_str());

    SetWindowText(hEditShotgunAmmo, std::to_wstring(player->shotgunAmmo).c_str());

    SetWindowText(hEditMachinegunAmmo, std::to_wstring(player->machineAmmo).c_str());

    SetWindowText(hEditRocketlauncherAmmo, std::to_wstring(player->rocketAmmo).c_str());

    SetWindowText(hEditRifleAmmo, std::to_wstring(player->rifleAmmo).c_str());

    SetWindowText(hEditGrenadelauncherAmmo, std::to_wstring(player->grenadeAmmo).c_str());

    SetWindowText(hEditPistolAmmo, std::to_wstring(player->pistolAmmo).c_str());

    SetWindowText(hEditFireballAmmo, std::to_wstring(player->fireBallAmmo).c_str());

    SetWindowText(hEditIceballAmmo, std::to_wstring(player->iceAmmo).c_str());

    SetWindowText(hEditLaserAmmo, std::to_wstring(player->laserAmmo).c_str());

    SetWindowText(hEditBiteAmmo, std::to_wstring(player->BiteAmmo).c_str());

    SetWindowText(hEditNosoundAmmo, std::to_wstring(player->skullAmmo).c_str());
}

static LRESULT CALLBACK MessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CLOSE:
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case SELECT_CHAINSAW:
            selectWeapon(0);
            break;
        case SELECT_SHOTGUN:
            selectWeapon(1);
            break;
        case SELECT_MACHINEGUN:
            selectWeapon(2);
            break;
        case SELECT_ROCKETLAUNCHER:
            selectWeapon(3);
            break;
        case SELECT_RIFLE:
            selectWeapon(4);
            break;
        case SELECT_GRENADELAUNCHER:
            selectWeapon(5);
            break;
        case SELECT_PISTOL:
            selectWeapon(6);
            break;
        case SELECT_FIREBALL:
            selectWeapon(7);
            break;
        case SELECT_ICEBALL:
            selectWeapon(8);
            break;
        case SELECT_LASER:
            selectWeapon(9);
            break;
        case SELECT_BITE:
            selectWeapon(10);
            break;
        case SELECT_NOSOUND:
            selectWeapon(11);
            break;
        case FREEZE_AMMO:
            isAmmoFrozen = !isAmmoFrozen;
            if (isAmmoFrozen)
                freezeAmmo->Enable();
            else
                freezeAmmo->Disable();

            SetWindowText(hFreezeAmmoBtn, isAmmoFrozen ? L"Unfreeze Ammo" : L"Freeze Ammo");
            break;
        case GOD_MODE:
            isGodModeEnabled = !isGodModeEnabled;
            if (isGodModeEnabled)
                godMode->Enable();
            else
                godMode->Disable();

            SetWindowText(hGodModeBtn, isGodModeEnabled ? L"Disable God Mode" : L"Enable God Mode");
            break;
        case RAPID_FIRE:
            isRapidFireEnabled = !isRapidFireEnabled;
            if (isRapidFireEnabled)
                rapidFire->Enable();
            else
                rapidFire->Disable();

            SetWindowText(hRapidFireBtn, isRapidFireEnabled ? L"Disable Rapid Fire" : L"Enable Rapid Fire");
            break;
        case TRIGGER_BOT:
            isTriggerBotEnabled = !isTriggerBotEnabled;
            SetWindowText(hTriggerBotBtn, isTriggerBotEnabled ? L"Disable Trigger Bot" : L"Enable Trigger Bot");
            break;
        case NO_RECOIL:
            isNoRecoilEnabled = !isNoRecoilEnabled;
            SetWindowText(hNoRecoilBtn, isNoRecoilEnabled ? L"Disable No Recoil" : L"Enable No Recoil");
            if (isNoRecoilEnabled)
                noRecoilPatch->Enable();
            else
                noRecoilPatch->Disable();
            break;
        case AIMBOT:
            isAimbotEnabled = !isAimbotEnabled;
            SetWindowText(hAimbotBtn, isAimbotEnabled ? L"Disable Aimbot" : L"Enable Aimbot");
            break;
        case UPDATE:
            updateAmmoValues();
            break;
        }
    case EN_CHANGE: {
        static int ammoValue[12];
        int tmpAmmoValue[12];
        switch (LOWORD(wParam))
        {
        case EDIT_CHAINSAW_AMMO:
            GetWindowText(hEditChainsawAmmo, buffer, 16);
            tmpAmmoValue[0] = _ttoi(buffer);

            if (ammoValue[0] != tmpAmmoValue[0])
            {
                ammoValue[0] = tmpAmmoValue[0];
                player->chainSaw = ammoValue[0];
            }
            break;

        case EDIT_SHOTGUN_AMMO:
            GetWindowText(hEditShotgunAmmo, buffer, 16);
            tmpAmmoValue[1] = _ttoi(buffer);

            if (ammoValue[1] != tmpAmmoValue[1])
            {
                ammoValue[1] = tmpAmmoValue[1];
                player->shotgunAmmo = ammoValue[1];
            }
            break;

        case EDIT_MACHINEGUN_AMMO:
            GetWindowText(hEditMachinegunAmmo, buffer, 16);
            tmpAmmoValue[2] = _ttoi(buffer);

            if (ammoValue[2] != tmpAmmoValue[2])
            {
                ammoValue[2] = tmpAmmoValue[2];
                player->machineAmmo = ammoValue[2];
            }
            break;

        case EDIT_ROCKETLAUNCHER_AMMO:
            GetWindowText(hEditRocketlauncherAmmo, buffer, 16);
            tmpAmmoValue[3] = _ttoi(buffer);

            if (ammoValue[3] != tmpAmmoValue[3])
            {
                ammoValue[3] = tmpAmmoValue[3];
                player->rocketAmmo = ammoValue[3];
            }
            break;

        case EDIT_RIFLE_AMMO:
            GetWindowText(hEditRifleAmmo, buffer, 16);
            tmpAmmoValue[4] = _ttoi(buffer);

            if (ammoValue[4] != tmpAmmoValue[4])
            {
                ammoValue[4] = tmpAmmoValue[4];
                player->rifleAmmo = ammoValue[4];
            }
            break;

        case EDIT_GRENADELAUNCHER_AMMO:
            GetWindowText(hEditGrenadelauncherAmmo, buffer, 16);
            tmpAmmoValue[5] = _ttoi(buffer);

            if (ammoValue[5] != tmpAmmoValue[5])
            {
                ammoValue[5] = tmpAmmoValue[5];
                player->grenadeAmmo = ammoValue[5];
            }
            break;

        case EDIT_PISTOL_AMMO:
            GetWindowText(hEditPistolAmmo, buffer, 16);
            tmpAmmoValue[6] = _ttoi(buffer);

            if (ammoValue[6] != tmpAmmoValue[6])
            {
                ammoValue[6] = tmpAmmoValue[6];
                player->pistolAmmo = ammoValue[6];
            }
            break;

        case EDIT_FIREBALL_AMMO:
            GetWindowText(hEditFireballAmmo, buffer, 16);
            tmpAmmoValue[7] = _ttoi(buffer);

            if (ammoValue[7] != tmpAmmoValue[7])
            {
                ammoValue[7] = tmpAmmoValue[7];
                player->fireBallAmmo = ammoValue[7];
            }
            break;

        case EDIT_ICEBALL_AMMO:
            GetWindowText(hEditIceballAmmo, buffer, 16);
            tmpAmmoValue[8] = _ttoi(buffer);

            if (ammoValue[8] != tmpAmmoValue[8])
            {
                ammoValue[8] = tmpAmmoValue[8];
                player->iceAmmo = ammoValue[8];
            }
            break;

        case EDIT_LASER_AMMO:
            GetWindowText(hEditLaserAmmo, buffer, 16);
            tmpAmmoValue[9] = _ttoi(buffer);

            if (ammoValue[9] != tmpAmmoValue[9])
            {
                ammoValue[9] = tmpAmmoValue[9];
                player->laserAmmo = ammoValue[9];
            }
            break;

        case EDIT_BITE_AMMO:
            GetWindowText(hEditBiteAmmo, buffer, 16);
            tmpAmmoValue[10] = _ttoi(buffer);

            if (ammoValue[10] != tmpAmmoValue[10])
            {
                ammoValue[10] = tmpAmmoValue[10];
                player->BiteAmmo = ammoValue[10];
            }
            break;

        case EDIT_NOSOUND_AMMO:
            GetWindowText(hEditNosoundAmmo, buffer, 16);
            tmpAmmoValue[11] = _ttoi(buffer);

            if (ammoValue[11] != tmpAmmoValue[11])
            {
                ammoValue[11] = tmpAmmoValue[11];
                player->skullAmmo = ammoValue[11];
            }
            break;
        }
        break;
    }
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

static bool triggerBotRunning = true;
Player* bestTarget = nullptr;
float bestDistance;

std::chrono::duration<float> deltaTime;
std::chrono::time_point<std::chrono::steady_clock> lastTime;
float lockTime = 0.0f;

static Player* MyTraceLine(Vec3* from, Vec3* to, Player* player, float* distance)
{
    Player* hitEnt = nullptr;

    Vec3 fromCpy = *from;
    Vec3* fromPos = &fromCpy;

    Vec3 toCpy = *to;
    Vec3* toPos = &toCpy;

    __asm {
            mov ecx, fromPos;
            mov edx, toPos;
            push player;
            push distance;
            call TraceLine;
            add esp, 0x8;
            mov [hitEnt], eax;
    }

    return hitEnt;
}

static void AimBot()
{
    float* distance{};

    auto tmpTime = std::chrono::high_resolution_clock::now();
    deltaTime = tmpTime - lastTime;
    lastTime = tmpTime;

    if (isAimbotEnabled && GetAsyncKeyState(VK_RBUTTON))
    {
        if (*entityCount == tempSize)
        {
            if (bestTarget && bestTarget != MyTraceLine(&player->headPos, &bestTarget->pos, player, distance))
            {
                lockTime = 0.0f;
            }

            if (lockTime < 0.01f)
            {
                bestTarget = nullptr;

                for (int i = 0; i < tempSize; ++i)
                {
                    if (entityList[i] == player)
                        continue;

                    if (!strcmp(entityList[i]->teamName, player->teamName))
                        continue;

                    Player* target = MyTraceLine(&player->headPos, &entityList[i]->pos, player, distance);
                    bool seen = target && target == entityList[i];

                    if (seen && GL::WorldToScreen(entityList[i]->pos, screenPos, viewMatrix))
                    {
                        GLint viewport[4];
                        glGetIntegerv(GL_VIEWPORT, viewport);

                        float centerY = viewport[3] / 2.0f;
                        float centerX = viewport[2] / 2.0f;

                        float distX = screenPos.x - centerX;
                        float distY = screenPos.y - centerY;
                        float tmpDistance = sqrtf((distX * distX) + (distY * distY));

                        // if distance is far away
                        if (tmpDistance > 300)
                            continue;

                        if (bestTarget)
                        {
                            if (tmpDistance < bestDistance)
                            {
                                bestTarget = entityList[i];
                                bestDistance = tmpDistance;
                            }
                        }
                        else
                        {
                            bestTarget = entityList[i];
                            bestDistance = tmpDistance;
                        }
                    }
                }
                if (bestTarget)
                {
                    lockTime = 0.001f;
                }
            }
            else
            {
                lockTime -= deltaTime.count();
            }

            if (bestTarget)
            {
                if (player->weaponCooldown < 1500)
                {
                    float rotX =
                        asinf((bestTarget->pos.z - player->headPos.z) / player->headPos.distance(bestTarget->pos)) /
                        (float)M_PI * 180.0f;
                    float rotY = -atan2f(bestTarget->pos.x - player->headPos.x, bestTarget->pos.y - player->headPos.y) /
                                 (float)M_PI * 180.0f;

                    float diffX = fmodf(rotX - player->rotX, 180.0f); // get the shortest angle
                    float diffY = fmodf(rotY - player->rotY, 180.0f); // get the shortest angle
                    diffX = fmodf(2 * diffX, 180.0f) - diffX;
                    diffY = fmodf(2 * diffY, 180.0f) - diffY;

                    float aimSpeed = 20.0f;
                    aimSpeed *= deltaTime.count();

                    float percision = 0.2f;
                    percision *= percision;

                    if (diffX * diffX > percision)
                        player->rotX += diffX * aimSpeed;
                    if (diffY * diffY > percision)
                        player->rotY += diffY * aimSpeed;

                    if (bestTarget == MyTraceLine(&player->headPos, worldpos, player, distance))
                    {
                        player->isShooting = true;

                        // // press left click
                        // INPUT input;
                    // input.type = INPUT_MOUSE;
                    // input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
                    // SendInput(1, &input, sizeof(INPUT));

                    // Sleep(10);

                        // input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
                        // SendInput(1, &input, sizeof(INPUT));
                    }
                    else
                    {
                        player->isShooting = false;
                    }
                }
            }
            else
            {
                player->isShooting = false;
            }
        }
        else
        {
            tempSize = *entityCount;
        }
    }
    else
    {
        player->isShooting = false;
        lockTime = 0.0f;
    }
}

static void triggerBot()
{
    Player* hitEnt = nullptr;
    float* distance{};

    while (triggerBotRunning)
    {
        if (isTriggerBotEnabled)
        {
            if (!player || !worldpos)
            {
                Sleep(2000);
                continue;
            }

            hitEnt = MyTraceLine(&player->headPos, worldpos, player, distance);

            if (hitEnt)
            {

                // hitEnt->hp = 1;
                player->isShooting = 1;
            }
            else
            {
                player->isShooting = 0;
            }
            Sleep(50);
        }
    }
}

BOOL _stdcall hwglSwapBuffers(HDC hDc)
{

    hookMtx.lock();
    int pixelformat = GetPixelFormat(hDc);

    // save old context and create new context
    HGLRC oldctx = wglGetCurrentContext();
    HDC oldhdc = wglGetCurrentDC();

    if (!contexts.count(pixelformat))
    {
        HGLRC myContext = wglCreateContext(hDc);
        HGLRC gameContext = wglGetCurrentContext();
        HDC old_dc = wglGetCurrentDC();
        wglMakeCurrent(hDc, myContext);

        wglMakeCurrent(old_dc, gameContext);
        contexts[pixelformat] = myContext;
    }

    wglMakeCurrent(hDc, contexts[pixelformat]);

    GL::SetupOrtho();

    if (*entityCount == tempSize)
    {
        for (int i = 0; i < *entityCount; i++)
        {
            if (entityList[i] == player)
                continue;

            if (entityList[i]->hp <= 0)
                continue;

            tmpWorldPos = entityList[i]->pos;
            tmpWorldPos.z -= 15.0f;

            playerPos = player->headPos;

            float distance = playerPos.distance(tmpWorldPos);

            if (distance > 5.0f && GL::WorldToScreen(tmpWorldPos, screenPos, viewMatrix))
            {
                if (strcmp(entityList[i]->teamName, player->teamName) == 0)
                {
                    // same team
                    GL::DrawEspBox(screenPos.x - 10, screenPos.y - 10, distance, rgb::green, entityList[i]->hp);
                }
                else
                { // enemy team
                    GL::DrawEspBox(screenPos.x - 10, screenPos.y - 10, distance, rgb::red, entityList[i]->hp);
                }
            }
        }
    }
    else
    {
        tempSize = *entityCount;
    }

    GL::RestoreGL();

    AimBot();

    // restore to old context
    wglMakeCurrent(oldhdc, oldctx);

    BOOL retValue = ((twglSwapBuffers)espHook->GetGateway())(hDc);

    hookMtx.unlock();

    return retValue;
}

DWORD WINAPI InternalMain(HMODULE hModule)
{
    AllocConsole();
    FILE* fDummy;
    freopen_s(&fDummy, "CONOUT$", "w", stdout);
    freopen_s(&fDummy, "CONOUT$", "w", stderr);

    const char gameName[] = "sauerbraten.exe";

    size_t playerOffset = 0x21E89C;

    uintptr_t moduleBase = GetModuleBaseAddress(gameName);

    player = *reinterpret_cast<Player**>(moduleBase + playerOffset);

    TraceLine = reinterpret_cast<TraceLine_t>(moduleBase + 0x1A2160);

    worldpos = reinterpret_cast<Vec3*>(moduleBase + 0x2A2C30);

    std::cout << "Module Base: " << std::hex << moduleBase << std::endl;
    std::thread triggerBotThread(triggerBot);

    std::cout << "Module Base: " << std::hex << moduleBase << std::endl;
    std::cout << "Player Offset: " << std::hex << playerOffset << std::endl;
    std::cout << "Player Address: " << std::hex << player << std::dec << std::endl;
    std::cout << "Player name: " << player->name << std::endl;

    freezeAmmo = std::make_shared<NopInternal>((BYTE*)(moduleBase + 0x1DB5E0), 8);
    rapidFire = std::make_shared<NopInternal>((BYTE*)(moduleBase + 0x1DBA02), 7);
    godMode = std::make_shared<NopInternal>((BYTE*)(moduleBase + 0x1DE446), 8);

    entityList = *reinterpret_cast<Player***>(moduleBase + 0x2B80F8);
    entityCount = reinterpret_cast<int*>(moduleBase + 0x2B8100);
    viewMatrix = reinterpret_cast<float*>(moduleBase + 0x2A3D70); // cam project view matrix

    BYTE noRecoilByteCode[] = "\x0F\x57\xC0\x90\x90\x90\x90\x90";
    noRecoilPatch = std::make_shared<ManagedPatch>((BYTE*)(moduleBase + 0x1A251F), noRecoilByteCode, 8);

    HMODULE hOpenGL32 = GetModuleHandleA("opengl32.dll");
    if (!hOpenGL32)
    {
        std::cerr << "Failed to get opengl32.dll" << std::endl;
        return -1;
    }

    void* wglSwapBuffersAddr = GetProcAddress(hOpenGL32, "wglSwapBuffers");

    if (!wglSwapBuffersAddr)
    {
        std::cerr << "Failed to get wglSwapBuffers" << std::endl;
        return -1;
    }

    espHook = std::make_shared<TrampHook>(wglSwapBuffersAddr, hwglSwapBuffers, 5);

    std::cout << "Made ESP Hook" << std::endl;
    espHook->Enable();

    HINSTANCE hInstance = GetModuleHandle(0);
    WNDCLASSEX wc{};
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = MessageHandler;
    wc.hInstance = hInstance;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszClassName = L"Cube2 External";

    RegisterClassEx(&wc);

    HWND hWnd = CreateWindowEx(0, L"Cube2 External", L"Cube2 External", WS_VISIBLE | WS_EX_LAYERED | WS_BORDER,
                               CW_USEDEFAULT, CW_USEDEFAULT, 500, 700, 0, 0, hInstance, 0);

    if (hWnd == nullptr)
    {
        std::cerr << "Failed to create window" << std::endl;
        DWORD error = GetLastError();
        std::cerr << "Error: L" << error << std::endl;
        return 1;
    }

    size_t yOffset = 0;
    size_t spacePeritem = 50;

    CreateWindowEx(0, L"Static", L"Chainsaw:", WS_TABSTOP | WS_VISIBLE | WS_CHILD, 10, 5 + yOffset, 200, 45, hWnd,
                   nullptr, hInstance, 0);

    CreateWindowEx(0, L"Button", L"Select", WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | BS_FLAT, 100,
                   15 + yOffset, 100, 30, hWnd, (HMENU)SELECT_CHAINSAW, hInstance, nullptr);

    hEditChainsawAmmo = CreateWindowEx(0, L"Edit", L"Ammo", WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER, 15,
                                       25 + yOffset, 60, 20, hWnd, (HMENU)EDIT_CHAINSAW_AMMO, hInstance, nullptr);

    yOffset = 1;
    yOffset += spacePeritem;
    CreateWindowEx(0, L"Static", L"Shotgun:", WS_TABSTOP | WS_VISIBLE | WS_CHILD, 10, 5 + yOffset, 200, 45, hWnd,
                   nullptr, hInstance, 0);

    CreateWindowEx(0, L"Button", L"Select", WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | BS_FLAT, 100,
                   15 + yOffset, 100, 30, hWnd, (HMENU)SELECT_SHOTGUN, hInstance, nullptr);

    hEditShotgunAmmo = CreateWindowEx(0, L"Edit", L"Ammo", WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER, 15,
                                      25 + yOffset, 60, 20, hWnd, (HMENU)EDIT_SHOTGUN_AMMO, hInstance, nullptr);

    yOffset = 2;
    yOffset *= spacePeritem;

    CreateWindowEx(0, L"Static", L"Machinegun:", WS_TABSTOP | WS_VISIBLE | WS_CHILD, 10, 5 + yOffset, 200, 45, hWnd,
                   nullptr, hInstance, 0);

    CreateWindowEx(0, L"Button", L"Select", WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | BS_FLAT, 100,
                   15 + yOffset, 100, 30, hWnd, (HMENU)SELECT_MACHINEGUN, hInstance, nullptr);

    hEditMachinegunAmmo = CreateWindowEx(0, L"Edit", L"Ammo", WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER, 15,
                                         25 + yOffset, 60, 20, hWnd, (HMENU)EDIT_MACHINEGUN_AMMO, hInstance, nullptr);

    yOffset = 3;
    yOffset *= spacePeritem;

    CreateWindowEx(0, L"Static", L"Rocket Launcher:", WS_TABSTOP | WS_VISIBLE | WS_CHILD, 10, 5 + yOffset, 200, 45,
                   hWnd, nullptr, hInstance, 0);

    CreateWindowEx(0, L"Button", L"Select", WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | BS_FLAT, 100,
                   15 + yOffset, 100, 30, hWnd, (HMENU)SELECT_ROCKETLAUNCHER, hInstance, nullptr);

    hEditRocketlauncherAmmo =
        CreateWindowEx(0, L"Edit", L"Ammo", WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER, 15, 25 + yOffset, 60, 20,
                       hWnd, (HMENU)EDIT_ROCKETLAUNCHER_AMMO, hInstance, nullptr);

    yOffset = 4;
    yOffset *= spacePeritem;

    CreateWindowEx(0, L"Static", L"Rifle:", WS_TABSTOP | WS_VISIBLE | WS_CHILD, 10, 5 + yOffset, 200, 45, hWnd, nullptr,
                   hInstance, 0);

    CreateWindowEx(0, L"Button", L"Select", WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | BS_FLAT, 100,
                   15 + yOffset, 100, 30, hWnd, (HMENU)SELECT_RIFLE, hInstance, nullptr);

    hEditRifleAmmo = CreateWindowEx(0, L"Edit", L"Ammo", WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER, 15,
                                    25 + yOffset, 60, 20, hWnd, (HMENU)EDIT_RIFLE_AMMO, hInstance, nullptr);

    yOffset = 5;
    yOffset *= spacePeritem;

    CreateWindowEx(0, L"Static", L"Grenade Launcher:", WS_TABSTOP | WS_VISIBLE | WS_CHILD, 10, 5 + yOffset, 200, 45,
                   hWnd, nullptr, hInstance, 0);

    CreateWindowEx(0, L"Button", L"Select", WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | BS_FLAT, 100,
                   15 + yOffset, 100, 30, hWnd, (HMENU)SELECT_GRENADELAUNCHER, hInstance, nullptr);

    hEditGrenadelauncherAmmo =
        CreateWindowEx(0, L"Edit", L"Ammo", WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER, 15, 25 + yOffset, 60, 20,
                       hWnd, (HMENU)EDIT_GRENADELAUNCHER_AMMO, hInstance, nullptr);

    yOffset = 6;
    yOffset *= spacePeritem;

    CreateWindowEx(0, L"Static", L"Pistol:", WS_TABSTOP | WS_VISIBLE | WS_CHILD, 10, 5 + yOffset, 200, 45, hWnd,
                   nullptr, hInstance, 0);

    CreateWindowEx(0, L"Button", L"Select", WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | BS_FLAT, 100,
                   15 + yOffset, 100, 30, hWnd, (HMENU)SELECT_PISTOL, hInstance, nullptr);

    hEditPistolAmmo = CreateWindowEx(0, L"Edit", L"Ammo", WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER, 15,
                                     25 + yOffset, 60, 20, hWnd, (HMENU)EDIT_PISTOL_AMMO, hInstance, nullptr);

    yOffset = 7;
    yOffset *= spacePeritem;

    CreateWindowEx(0, L"Static", L"Fireball:", WS_TABSTOP | WS_VISIBLE | WS_CHILD, 10, 5 + yOffset, 200, 45, hWnd,
                   nullptr, hInstance, 0);

    CreateWindowEx(0, L"Button", L"Select", WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | BS_FLAT, 100,
                   15 + yOffset, 100, 30, hWnd, (HMENU)SELECT_FIREBALL, hInstance, nullptr);

    hEditFireballAmmo = CreateWindowEx(0, L"Edit", L"Ammo", WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER, 15,
                                       25 + yOffset, 60, 20, hWnd, (HMENU)EDIT_FIREBALL_AMMO, hInstance, nullptr);

    yOffset = 8;
    yOffset *= spacePeritem;

    CreateWindowEx(0, L"Static", L"Iceball:", WS_TABSTOP | WS_VISIBLE | WS_CHILD, 10, 5 + yOffset, 200, 45, hWnd,
                   nullptr, hInstance, 0);

    CreateWindowEx(0, L"Button", L"Select", WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | BS_FLAT, 100,
                   15 + yOffset, 100, 30, hWnd, (HMENU)SELECT_ICEBALL, hInstance, nullptr);

    hEditIceballAmmo = CreateWindowEx(0, L"Edit", L"Ammo", WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER, 15,
                                      25 + yOffset, 60, 20, hWnd, (HMENU)EDIT_ICEBALL_AMMO, hInstance, nullptr);

    yOffset = 9;
    yOffset *= spacePeritem;

    CreateWindowEx(0, L"Static", L"Laser:", WS_TABSTOP | WS_VISIBLE | WS_CHILD, 10, 5 + yOffset, 200, 45, hWnd, nullptr,
                   hInstance, 0);

    CreateWindowEx(0, L"Button", L"Select", WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | BS_FLAT, 100,
                   15 + yOffset, 100, 30, hWnd, (HMENU)SELECT_LASER, hInstance, nullptr);

    hEditLaserAmmo = CreateWindowEx(0, L"Edit", L"Ammo", WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER, 15,
                                    25 + yOffset, 60, 20, hWnd, (HMENU)EDIT_LASER_AMMO, hInstance, nullptr);

    yOffset = 10;
    yOffset *= spacePeritem;

    CreateWindowEx(0, L"Static", L"Bite:", WS_TABSTOP | WS_VISIBLE | WS_CHILD, 10, 5 + yOffset, 200, 45, hWnd, nullptr,
                   hInstance, 0);

    CreateWindowEx(0, L"Button", L"Select", WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | BS_FLAT, 100,
                   15 + yOffset, 100, 30, hWnd, (HMENU)SELECT_BITE, hInstance, nullptr);

    hEditBiteAmmo = CreateWindowEx(0, L"Edit", L"Ammo", WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER, 15,
                                   25 + yOffset, 60, 20, hWnd, (HMENU)EDIT_BITE_AMMO, hInstance, nullptr);

    yOffset = 11;
    yOffset *= spacePeritem;

    CreateWindowEx(0, L"Static", L"No Sound:", WS_TABSTOP | WS_VISIBLE | WS_CHILD, 10, 5 + yOffset, 200, 45, hWnd,
                   nullptr, hInstance, 0);

    CreateWindowEx(0, L"Button", L"Select", WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | BS_FLAT, 100,
                   15 + yOffset, 100, 30, hWnd, (HMENU)SELECT_NOSOUND, hInstance, nullptr);

    hEditNosoundAmmo = CreateWindowEx(0, L"Edit", L"Ammo", WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER, 15,
                                      25 + yOffset, 60, 20, hWnd, (HMENU)EDIT_NOSOUND_AMMO, hInstance, nullptr);

    yOffset = 12;
    yOffset *= spacePeritem;

    CreateWindowEx(0, L"Button", L"Update Ammo", WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | BS_FLAT, 220,
                   yOffset, 200, 50, hWnd, (HMENU)UPDATE, hInstance, nullptr);

    spacePeritem = 60;

    yOffset = 0;
    yOffset *= spacePeritem;

    hFreezeAmmoBtn = CreateWindowEx(0, L"Button", L"Freeze Ammo", WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
                                    200, 10 + yOffset, 250, 50, hWnd, (HMENU)FREEZE_AMMO, hInstance, nullptr);

    yOffset = 1;
    yOffset *= spacePeritem;

    hGodModeBtn =
        CreateWindowEx(0, L"Button", L"Enable God Mode", WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 200,
                       10 + yOffset, 250, 50, hWnd, (HMENU)GOD_MODE, hInstance, nullptr);

    yOffset = 2;
    yOffset *= spacePeritem;

    hRapidFireBtn =
        CreateWindowEx(0, L"Button", L"Enable Rapid Fire", WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 200,
                       10 + yOffset, 250, 50, hWnd, (HMENU)RAPID_FIRE, hInstance, nullptr);

    yOffset = 3;
    yOffset *= spacePeritem;

    hTriggerBotBtn =
        CreateWindowEx(0, L"Button", L"Enable Trigger Bot", WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 200,
                       10 + yOffset, 250, 50, hWnd, (HMENU)TRIGGER_BOT, hInstance, nullptr);

    yOffset = 4;
    yOffset *= spacePeritem;

    hNoRecoilBtn =
        CreateWindowEx(0, L"Button", L"Enable No Recoil", WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 200,
                       10 + yOffset, 250, 50, hWnd, (HMENU)NO_RECOIL, hInstance, nullptr);

    yOffset = 5;
    yOffset *= spacePeritem;

    hAimbotBtn = CreateWindowEx(0, L"Button", L"Enable Aimbot", WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
                                200, 10 + yOffset, 250, 50, hWnd, (HMENU)AIMBOT, hInstance, nullptr);

    updateAmmoValues();
    MSG msg;
    DWORD dExit = 0;

    isInitialized = true;

    while (true)
    {
        BOOL result = GetMessage(&msg, hWnd, 0, 0);
        if (result > 0)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            return result;
        }
    }

    fclose(fDummy);
    FreeConsole();

    triggerBotRunning = false;
    triggerBotThread.join();

    FreeLibraryAndExitThread(hModule, 0);

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        HANDLE tHndl = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)InternalMain, hModule, 0, nullptr);

        if (tHndl)
            CloseHandle(tHndl);
        else
            return FALSE;
        break;
    }
    return TRUE;
}
