#pragma once

#include <cstdint>

// Created with ReClass.NET 1.2 by KN4CK3R

struct Vec3
{
    union {
        struct
        {
            float x, y, z;
        };
        struct
        {
            float r, g, b;
        };
        float v[3];
    };
};

class Player
{
  public:
    // float posX;             // 0x0000
    // float posZ;             // 0x0004
    // float posY;             // 0x0008
    // float velX;             // 0x000C
    // float velZ;             // 0x0010
    // float velY;             // 0x0014
    Vec3 pos;
    Vec3 vel;
    float forceForward;     // 0x0018
    float forceBack;        // 0x001C
    float forceUp;          // 0x0020
    int8_t pad_0024[12];    // 0x0024
    float hPosX;            // 0x0030
    float hPosZ;            // 0x0034
    float hPosY;            // 0x0038
    float lookSide;         // 0x003C
    float lookUp;           // 0x0040
    int8_t pad_0044[4];     // 0x0044
    float walkSpeed;        // 0x0048
    int8_t pad_004C[36];    // 0x004C
    int32_t status;         // 0x0070
    int8_t pad_0074[260];   // 0x0074
    int32_t hp;             // 0x0178
    int32_t maxHp;          // 0x017C
    int32_t armour;         // 0x0180
    int32_t armourType;     // 0x0184
    int32_t quadTime;       // 0x0188
    int32_t selectedGun;    // 0x018C
    int32_t weaponCooldown; // 0x0190
    int32_t chainSaw;       // 0x0194
    int32_t shotgunAmmo;    // 0x0198
    int32_t machineAmmo;    // 0x019C
    int32_t rocketAmmo;     // 0x01A0
    int32_t rifleAmmo;      // 0x01A4
    int32_t grenadeAmmo;    // 0x01A8
    int32_t pistolAmmo;     // 0x01AC
    int32_t fireBallAmmo;   // 0x01B0
    int32_t iceAmmo;        // 0x01B4
    int32_t laserAmmo;      // 0x01B8
    int32_t BiteAmmo;       // 0x01BC
    int32_t skullAmmo;      // 0x01C0
    int32_t N00000494;      // 0x01C4
    int32_t N00000486;      // 0x01C8
    int32_t mass;           // 0x01CC
    int32_t c;              // 0x01D0
    int8_t pad_01D4[40];    // 0x01D4
    int8_t isShooting;      // 0x0FC
};

typedef Player*(__cdecl* TraceLine_t)(Vec3* to, Vec3* from, Player* player, float& dist);