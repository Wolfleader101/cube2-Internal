#pragma once

#pragma comment(lib, "opengl32.lib")
#include <Windows.h>
#include <gl/GL.h>

#include <chrono>
#include <string>

#include <imgui.h>

#include "Math/Math.hpp"

namespace rgb
{
    constexpr const ImColor red{255, 0, 0};
    constexpr const ImColor green{0, 255, 0};
    constexpr const ImColor blue{0, 0, 255};
    constexpr const ImColor white{255, 255, 255};
    constexpr const ImColor black{0, 0, 0};
    constexpr const ImColor yellow{255, 255, 0};
    constexpr const ImColor purple{255, 0, 255};
    constexpr const ImColor cyan{0, 255, 255};
    constexpr const ImColor orange{255, 165, 0};
    constexpr const ImColor pink{255, 192, 203};
    constexpr const ImColor gray{128, 128, 128};
    constexpr const ImColor darkGray{169, 169, 169};
    constexpr const ImColor lightGray{211, 211, 211};
    constexpr const ImColor brown{165, 42, 42};
    constexpr const ImColor darkGreen{0, 100, 0};
    constexpr const ImColor darkRed{139, 0, 0};
    constexpr const ImColor darkBlue{0, 0, 139};
    constexpr const ImColor darkYellow{139, 139, 0};
    constexpr const ImColor darkPurple{139, 0, 139};
    constexpr const ImColor darkCyan{0, 139, 139};
    constexpr const ImColor darkOrange{255, 140, 0};
    constexpr const ImColor darkPink{199, 21, 133};
    constexpr const ImColor darkBrown{139, 69, 19};
    constexpr const ImColor lightGreen{144, 238, 144};
    constexpr const ImColor lightRed{255, 99, 71};
    constexpr const ImColor lightBlue{173, 216, 230};
    constexpr const ImColor lightYellow{255, 255, 224};
    constexpr const ImColor lightPurple{218, 112, 214};
    constexpr const ImColor lightCyan{224, 255, 255};
    constexpr const ImColor lightOrange{255, 218, 185};
    constexpr const ImColor lightPink{255, 0, 255};
    constexpr const ImColor lightBrown{205, 133, 63};
} // namespace rgb

namespace Renderer
{
    void Setup(void* deviceContext);

    void StartFrame();
    void EndFrame();

    bool WorldToScreen(const Math::Vec3& pos, Math::Vec2& screen, const float viewProj[16], float width, float height);

    float GetDeltaTimeSeconds();

    namespace Draw
    {
        void Start();

        void End();

        void Line(const Math::Vec2& from, const Math::Vec2& to, const ImColor& color, float thickness = 1.0f,
                  bool outline = false);
        void Text(const std::string& text, const Math::Vec2& pos, const ImColor& color, float size, bool center = false,
                  bool outline = false);
        void Rect(const Math::Vec2& upperLeft, const Math::Vec2& lowerRight, const ImColor& color,
                  float thickness = 1.0f, bool outline = false);
        void Circle(const Math::Vec2& center, float radius, const ImColor& color, float thickness = 1.0f,
                    bool outline = false);

        void EspBox(const Math::Vec2& pos, float height, float distance, const ImColor& color,
                    const int healthPercent = -1, bool snapLines = false, bool outline = false);
    } // namespace Draw
} // namespace Renderer
