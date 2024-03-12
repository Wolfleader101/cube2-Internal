#include "Renderer/Renderer.hpp"

#include <firaCode.hpp>
#include <imgui_impl_opengl2.h>
#include <imgui_impl_win32.h>

std::chrono::duration<float> deltaTime{};
std::chrono::time_point<std::chrono::steady_clock> lastTime{};

void SetupImGuiStyle()
{
    // Future Dark style by rewrking from ImThemes
    ImGuiStyle& style = ImGui::GetStyle();

    style.Alpha = 1.0f;
    style.DisabledAlpha = 1.0f;
    style.WindowPadding = ImVec2(12.0f, 12.0f);
    style.WindowRounding = 0.0f;
    style.WindowBorderSize = 0.0f;
    style.WindowMinSize = ImVec2(20.0f, 20.0f);
    style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
    style.WindowMenuButtonPosition = ImGuiDir_None;
    style.ChildRounding = 0.0f;
    style.ChildBorderSize = 1.0f;
    style.PopupRounding = 0.0f;
    style.PopupBorderSize = 1.0f;
    style.FramePadding = ImVec2(6.0f, 6.0f);
    style.FrameRounding = 0.0f;
    style.FrameBorderSize = 0.0f;
    style.ItemSpacing = ImVec2(12.0f, 6.0f);
    style.ItemInnerSpacing = ImVec2(6.0f, 3.0f);
    style.CellPadding = ImVec2(12.0f, 6.0f);
    style.IndentSpacing = 20.0f;
    style.ColumnsMinSpacing = 6.0f;
    style.ScrollbarSize = 12.0f;
    style.ScrollbarRounding = 0.0f;
    style.GrabMinSize = 12.0f;
    style.GrabRounding = 0.0f;
    style.TabRounding = 0.0f;
    style.TabBorderSize = 0.0f;
    style.TabMinWidthForCloseButton = 0.0f;
    style.ColorButtonPosition = ImGuiDir_Right;
    style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
    style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

    style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.2745098173618317f, 0.3176470696926117f, 0.4509803950786591f, 1.0f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
    style.Colors[ImGuiCol_ChildBg] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.2352941185235977f, 0.2156862765550613f, 0.5960784554481506f, 1.0f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
    style.Colors[ImGuiCol_TitleBgActive] =
        ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
    style.Colors[ImGuiCol_TitleBgCollapsed] =
        ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.09803921729326248f, 0.105882354080677f, 0.1215686276555061f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] =
        ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] =
        ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.5372549295425415f, 0.5529412031173706f, 1.0f, 1.0f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 1.0f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.2352941185235977f, 0.2156862765550613f, 0.5960784554481506f, 1.0f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 1.0f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.2352941185235977f, 0.2156862765550613f, 0.5960784554481506f, 1.0f);
    style.Colors[ImGuiCol_Separator] = ImVec4(0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0f);
    style.Colors[ImGuiCol_SeparatorHovered] =
        ImVec4(0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0f);
    style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0f);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
    style.Colors[ImGuiCol_ResizeGripHovered] =
        ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 1.0f);
    style.Colors[ImGuiCol_ResizeGripActive] =
        ImVec4(0.2352941185235977f, 0.2156862765550613f, 0.5960784554481506f, 1.0f);
    style.Colors[ImGuiCol_Tab] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
    style.Colors[ImGuiCol_TabHovered] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
    style.Colors[ImGuiCol_TabActive] = ImVec4(0.09803921729326248f, 0.105882354080677f, 0.1215686276555061f, 1.0f);
    style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
    style.Colors[ImGuiCol_TabUnfocusedActive] =
        ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
    style.Colors[ImGuiCol_PlotLines] = ImVec4(0.5215686559677124f, 0.6000000238418579f, 0.7019608020782471f, 1.0f);
    style.Colors[ImGuiCol_PlotLinesHovered] =
        ImVec4(0.03921568766236305f, 0.9803921580314636f, 0.9803921580314636f, 1.0f);
    style.Colors[ImGuiCol_PlotHistogram] = ImVec4(1.0f, 0.2901960909366608f, 0.5960784554481506f, 1.0f);
    style.Colors[ImGuiCol_PlotHistogramHovered] =
        ImVec4(0.9960784316062927f, 0.4745098054409027f, 0.6980392336845398f, 1.0f);
    style.Colors[ImGuiCol_TableHeaderBg] =
        ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
    style.Colors[ImGuiCol_TableBorderStrong] =
        ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
    style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
    style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.09803921729326248f, 0.105882354080677f, 0.1215686276555061f, 1.0f);
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.2352941185235977f, 0.2156862765550613f, 0.5960784554481506f, 1.0f);
    style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
    style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
    style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
    style.Colors[ImGuiCol_NavWindowingDimBg] =
        ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 0.501960813999176f);
    style.Colors[ImGuiCol_ModalWindowDimBg] =
        ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 0.501960813999176f);
}

namespace Renderer
{
    void Setup(void* deviceContext)
    {
        ImGui::CreateContext();
        ImGui_ImplWin32_Init(deviceContext);
        ImGui_ImplOpenGL2_Init();
        ImGuiIO& io = ImGui::GetIO();
        io.Fonts->AddFontFromMemoryCompressedBase85TTF(FiraCode_compressed_data_base85, 16.0f, nullptr,
                                                       io.Fonts->GetGlyphRangesDefault());

        SetupImGuiStyle();
    }

    void StartFrame()
    {
        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        auto currentFrame = std::chrono::steady_clock::now();
        deltaTime = currentFrame - lastTime;
        lastTime = currentFrame;
    }

    void EndFrame()
    {
        ImGui::EndFrame();
        ImGui::Render();
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
    }

    bool WorldToScreen(const Math::Vec3& pos, Math::Vec2& screen, const float viewProj[16], float width, float height)
    {
        Math::Vec4 clipCoords;
        clipCoords.x = pos.x * viewProj[0] + pos.y * viewProj[4] + pos.z * viewProj[8] + viewProj[12];
        clipCoords.y = pos.x * viewProj[1] + pos.y * viewProj[5] + pos.z * viewProj[9] + viewProj[13];
        clipCoords.z = pos.x * viewProj[2] + pos.y * viewProj[6] + pos.z * viewProj[10] + viewProj[14];
        clipCoords.w = pos.x * viewProj[3] + pos.y * viewProj[7] + pos.z * viewProj[11] + viewProj[15];

        if (clipCoords.w < 0.1f)
            return false;

        Math::Vec3 NDC;
        NDC.x = clipCoords.x / clipCoords.w;
        NDC.y = clipCoords.y / clipCoords.w;
        NDC.z = clipCoords.z / clipCoords.w;

        screen.x = (NDC.x + 1.0f) * width / 2.0f;
        screen.y = (1.0f - NDC.y) * height / 2.0f;

        return true;
    }

    float GetDeltaTimeSeconds()
    {
        return deltaTime.count();
    }
} // namespace Renderer

namespace Renderer::Draw
{
    void Start()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.0f, 0.0f});
        ImGui::PushStyleColor(ImGuiCol_WindowBg, {0.0f, 0.0f, 0.0f, 0.0f});

        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y), ImGuiCond_Always);

        ImGui::Begin("##Canvas", nullptr,
                     ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus |
                         ImGuiWindowFlags_NoMouseInputs);
    }

    void End()
    {
        ImGui::GetWindowDrawList()->PushClipRectFullScreen();
        ImGui::End();
        ImGui::PopStyleColor();
        ImGui::PopStyleVar(2);
    }

    void Line(const Math::Vec2& from, const Math::Vec2& to, const ImColor& color, float thickness, bool outline)
    {
        if (outline)
            ImGui::GetWindowDrawList()->AddLine({from.x, from.y}, {to.x, to.y}, rgb::black, thickness + 2);

        ImGui::GetWindowDrawList()->AddLine({from.x, from.y}, {to.x, to.y}, color, thickness);
    }

    void Text(const std::string& text, const Math::Vec2& pos, const ImColor& color, float size, bool center,
              bool outline)
    {
        if (center)
        {
            auto textSize = ImGui::CalcTextSize(text.c_str());

            if (outline)
                ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), size + 2,
                                                    {pos.x - textSize.x / 2, pos.y - textSize.y / 2}, rgb::black,
                                                    text.c_str());

            ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), size,
                                                {pos.x - textSize.x / 2, pos.y - textSize.y / 2}, color, text.c_str());
            return;
        }

        if (outline)
            ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), size + 2, {pos.x, pos.y}, rgb::black, text.c_str());

        ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), size, {pos.x, pos.y}, color, text.c_str());
    }

    void Rect(const Math::Vec2& upperLeft, const Math::Vec2& lowerRight, const ImColor& color, float thickness,
              bool outline)
    {
        if (outline)
            ImGui::GetWindowDrawList()->AddRect({upperLeft.x, upperLeft.y}, {lowerRight.x, lowerRight.y}, rgb::black,
                                                0.0f, 0, thickness + 2);

        ImGui::GetWindowDrawList()->AddRect({upperLeft.x, upperLeft.y}, {lowerRight.x, lowerRight.y}, color, 0.0f, 0,
                                            thickness);
    }

    void Circle(const Math::Vec2& center, float radius, const ImColor& color, float thickness, bool outline)
    {
        if (outline)
            ImGui::GetWindowDrawList()->AddCircle({center.x, center.y}, radius, rgb::black, 0, thickness + 2);

        ImGui::GetWindowDrawList()->AddCircle({center.x, center.y}, radius, color, 0, thickness);
    }

    void EspBox(const Math::Vec2& pos, float charHeight, float distance, const ImColor& color, const int healthPercent,
                bool snapLines, bool outline)
    {
        float lineWidth = 1.0f;

        ImVec2 displaySize = ImGui::GetIO().DisplaySize;
        float height = (displaySize.y / distance) * charHeight;
        float width = height / 2.0f;

        // snaplines
        if (snapLines)
        {
            Line({displaySize.x / 2, displaySize.y}, {pos.x, pos.y}, color, lineWidth / 2, outline);
        }

        // boxx
        Rect({pos.x - width / 2, pos.y - height}, {pos.x + width / 2, pos.y}, color, lineWidth, outline);

        // healthbar bottom
        if (healthPercent != -1)
        {
            float perc = width / 100;
            float healthWidth = perc * healthPercent;

            // background
            Rect({pos.x - width / 2, pos.y + 1}, {pos.x + width / 2, pos.y + 3}, rgb::black, 0.0f, false);

            ImColor hColor = ImColor(healthPercent / 100.0f, healthPercent / 100.0f, 0.0f, 0.8f);
            Rect({pos.x - width / 2, pos.y + 1}, {pos.x - width / 2 + healthWidth, pos.y + 3}, hColor, 0.0f, false);
        }
    }
} // namespace Renderer::Draw