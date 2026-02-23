#include "font_manager.h"
#include <imgui.h>
#include <imgui-SFML.h>
#include <iostream>
#include <filesystem>

ImFont* FONT_TITLE = nullptr;
ImFont* FONT_BODY  = nullptr;

void LoadFonts()
{
    ImGuiIO& io = ImGui::GetIO();

    const std::string base =
        std::filesystem::current_path().parent_path().string();

    const std::string regularPath =
        base + "/fonts/Mali-Regular.ttf";

    const std::string semiBoldPath =
        base + "/fonts/Mali-SemiBold.ttf";

    std::cout << "Loading Thai fonts...\n";

    static ImVector<ImWchar> ranges; // เปลี่ยนเป็น static
    ImFontGlyphRangesBuilder builder;

    builder.AddRanges(io.Fonts->GetGlyphRangesDefault()); // อังกฤษ
    builder.AddRanges(io.Fonts->GetGlyphRangesThai());    // ไทย
    builder.BuildRanges(&ranges);

    FONT_BODY = io.Fonts->AddFontFromFileTTF(
        regularPath.c_str(),
        22.f,
        nullptr,
        ranges.Data
    );

    FONT_TITLE = io.Fonts->AddFontFromFileTTF(
        semiBoldPath.c_str(),
        36.f,
        nullptr,
        ranges.Data
    );

    if (!FONT_BODY || !FONT_TITLE)
    {
        std::cout << "Font load failed -> fallback\n";
        FONT_BODY = FONT_TITLE = io.Fonts->AddFontDefault();
    }
    else
    {
        std::cout << "Thai fonts loaded successfully ✅\n";
    }

    ImGui::SFML::UpdateFontTexture();
}