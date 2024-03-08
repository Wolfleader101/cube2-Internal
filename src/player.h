#pragma once

#include <cstdint>

#include "Math.hpp"
// Created with ReClass.NET 1.2 by KN4CK3R

class Player
{
  public:
    Vec3 pos;               // 0x0000
    Vec3 vel;               // 0x000C
    float forceForward;     // 0x0018
    float forceBack;        // 0x001C
    float forceUp;          // 0x0020
    int8_t pad_0024[12];    // 0x0024
    Vec3 headPos;           // 0x0030
    float rotY;             // 0x003C
    float rotX;             // 0x0040
    int8_t pad_0044[4];     // 0x0044
    float walkSpeed;        // 0x0048
    int8_t pad_004C[36];    // 0x004C
    int32_t status;         // 0x0070
    int8_t pad_0074[224];   // 0x0074
    int32_t hp;             // 0x0154
    int32_t maxHp;          // 0x0158
    int32_t armour;         // 0x015C
    int32_t armourType;     // 0x0160
    int32_t quadTime;       // 0x0164
    int32_t selectedGun;    // 0x0168
    int32_t weaponCooldown; // 0x016C
    int32_t chainSaw;       // 0x0170
    int32_t shotgunAmmo;    // 0x0174
    int32_t machineAmmo;    // 0x0178
    int32_t rocketAmmo;     // 0x017C
    int32_t rifleAmmo;      // 0x0180
    int32_t grenadeAmmo;    // 0x0184
    int32_t pistolAmmo;     // 0x0188
    int32_t fireBallAmmo;   // 0x018C
    int32_t iceAmmo;        // 0x0190
    int32_t laserAmmo;      // 0x0194
    int32_t BiteAmmo;       // 0x0198
    int32_t skullAmmo;      // 0x019C
    int32_t N00000494;      // 0x01A0
    int32_t N00000486;      // 0x01A4
    int32_t mass;           // 0x01A8
    int32_t c;              // 0x01AC
    int8_t pad_01B0[40];    // 0x01B0
    int8_t isShooting;      // 0x01D8
    int8_t pad_01D9[111];   // 0x01D9
    char name[16];          // 0x0248
    int8_t pad_0258[244];   // 0x0258
    char teamName[5];       // 0x034C
};                          // Size: 0x0351

typedef Player*(__cdecl* TraceLine_t)(Vec3* from, Vec3* to, const Player* player, float& dist);