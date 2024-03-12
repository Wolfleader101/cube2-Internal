#pragma once
#pragma comment(lib, "opengl32.lib")
#include <Windows.h>
#include <gl/GL.h>

#include "Math/Math.hpp"

namespace rgb
{
    constexpr const GLubyte red[3] = {255, 0, 0};
    constexpr const GLubyte green[3] = {0, 255, 0};
    constexpr const GLubyte blue[3] = {0, 0, 255};
    constexpr const GLubyte white[3] = {255, 255, 255};
    constexpr const GLubyte black[3] = {0, 0, 0};
    constexpr const GLubyte yellow[3] = {255, 255, 0};
    constexpr const GLubyte purple[3] = {255, 0, 255};
    constexpr const GLubyte cyan[3] = {0, 255, 255};
    constexpr const GLubyte orange[3] = {255, 165, 0};
    constexpr const GLubyte pink[3] = {255, 192, 203};
    constexpr const GLubyte gray[3] = {128, 128, 128};
    constexpr const GLubyte darkGray[3] = {169, 169, 169};
    constexpr const GLubyte lightGray[3] = {211, 211, 211};
    constexpr const GLubyte brown[3] = {165, 42, 42};
    constexpr const GLubyte darkGreen[3] = {0, 100, 0};
    constexpr const GLubyte darkRed[3] = {139, 0, 0};
    constexpr const GLubyte darkBlue[3] = {0, 0, 139};
    constexpr const GLubyte darkYellow[3] = {139, 139, 0};
    constexpr const GLubyte darkPurple[3] = {139, 0, 139};
    constexpr const GLubyte darkCyan[3] = {0, 139, 139};
    constexpr const GLubyte darkOrange[3] = {255, 140, 0};
    constexpr const GLubyte darkPink[3] = {199, 21, 133};
    constexpr const GLubyte darkBrown[3] = {139, 69, 19};
    constexpr const GLubyte lightGreen[3] = {144, 238, 144};
    constexpr const GLubyte lightRed[3] = {255, 99, 71};
    constexpr const GLubyte lightBlue[3] = {173, 216, 230};
    constexpr const GLubyte lightYellow[3] = {255, 255, 224};
    constexpr const GLubyte lightPurple[3] = {218, 112, 214};
    constexpr const GLubyte lightCyan[3] = {224, 255, 255};
    constexpr const GLubyte lightOrange[3] = {255, 218, 185};
    constexpr const GLubyte lightPink[3] = {255, 0, 255};
    constexpr const GLubyte lightBrown[3] = {205, 133, 63};
} // namespace rgb

namespace GL
{
    void SetupOrtho();
    void RestoreGL();
    bool WorldToScreen(const Math::Vec3& worldPos, Math::Vec2& screenPos, const float viewMatrix[16]);

    void DrawFilledRect(float x, float y, float width, float height, const GLubyte color[3]);
    void DrawOutline(float x, float y, float width, float height, float lineWidth, const GLubyte color[3]);
    void DrawLine(float fromX, float fromY, float toX, float toY, float lineWidth, const GLubyte color[3]);
    void DrawEspBox(float posX, float posY, float distance, const GLubyte color[3], const int healthPercent = -1);
} // namespace GL