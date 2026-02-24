#include "ui_mainmenu.h"
#include "FlowerRenderer.h"
#include "data_models.h"
#include "font_manager.h"
#include "config.h"

#include <imgui.h>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <string>

void drawMainMenu(
    sf::RenderWindow& window,
    AppState& state,
    UserSelection& selection
)
{
    static int occasionIndex = 0;
    static int containerIndex = 0;
    static float time = 0.f;
    static float anim = 0.f;

    time += 0.01f;
    anim = std::min(anim + 0.03f, 1.f);
    // ======================================================
    // Background Gradient
    // ======================================================
    sf::VertexArray bg(sf::PrimitiveType::TriangleStrip, 4);
    bg[0].position = {0.f, 0.f};
    bg[1].position = {(float)WINDOW_WIDTH, 0.f};
    bg[2].position = {0.f, (float)WINDOW_HEIGHT};
    bg[3].position = {(float)WINDOW_WIDTH, (float)WINDOW_HEIGHT};

    bg[0].color = BG_GRADIENT_TOP_LEFT;
    bg[1].color = BG_GRADIENT_TOP_RIGHT;
    bg[2].color = BG_GRADIENT_BOTTOM_LEFT;
    bg[3].color = BG_GRADIENT_BOTTOM_RIGHT;

    window.draw(bg);

    // ======================================================
    // Floating Flowers
    // ======================================================
    if (flowerList.size() >= 36)
    {
        FlowerRenderer::draw(window, flowerList[0].type,  {300, 260}, 120, time * 20);
        FlowerRenderer::draw(window, flowerList[13].type, {980, 260}, 140, -time * 15);
        FlowerRenderer::draw(window, flowerList[25].type, {640, 560}, 100, time * 10);
    }

    // ======================================================
    // ImGui Theme
    // ======================================================
    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_Text] = COLOR_TEXT;
    style.Colors[ImGuiCol_WindowBg] = COLOR_WINDOW_BG;
    style.Colors[ImGuiCol_Button] = COLOR_BUTTON;
    style.Colors[ImGuiCol_ButtonHovered] = COLOR_BUTTON_HOVER;
    style.Colors[ImGuiCol_ButtonActive] = COLOR_BUTTON_ACTIVE;
    style.Colors[ImGuiCol_FrameBg] = COLOR_FRAME_BG;
    style.Colors[ImGuiCol_FrameBgHovered] = COLOR_FRAME_BG_HOVER;
    style.Colors[ImGuiCol_FrameBgActive] = COLOR_FRAME_BG_ACTIVE;
    style.Colors[ImGuiCol_PopupBg] = COLOR_POPUP_BG;

    style.WindowRounding = ROUND_WINDOW;
    style.FrameRounding = ROUND_FRAME;
    style.PopupRounding = ROUND_POPUP;

    // ======================================================
    // Center Window
    // ======================================================
    // คำนวณจุดกึ่งกลางจากค่า WINDOW_WIDTH และ WINDOW_HEIGHT
    ImGui::SetNextWindowPos(
        ImVec2((float)WINDOW_WIDTH / 2.f, (float)WINDOW_HEIGHT / 2.f), 
        ImGuiCond_Always, 
        ImVec2(0.5f, 0.5f)
    );
    ImGui::SetNextWindowSize({480, 460});

    ImGui::Begin("Flower Boutique", nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoMove
    );

    // ======================================================
    // Title
    // ======================================================
    ImGui::PushFont(FONT_TITLE);
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, anim);
    
    // จัดตัวอักษรให้อยู่กึ่งกลางหน้าต่าง ImGui
    float titleWidth = ImGui::CalcTextSize("Flower Boutique").x;
    ImGui::SetCursorPosX((ImGui::GetWindowSize().x - titleWidth) * 0.5f);
    
    ImGui::Text("Flower Boutique");
    ImGui::PopStyleVar();
    ImGui::PopFont();

    ImGui::Separator();
    ImGui::PushFont(FONT_BODY);

    const char* occasionItems[] = {
        "Condolences",
        "Congratulations",
        "Love & Care"
    };

    ImGui::Text("Select Occasion");
    ImGui::Combo("##Occasion", &occasionIndex, occasionItems, IM_ARRAYSIZE(occasionItems));

    ImGui::Spacing();
    ImGui::Text("Select Container");

    std::vector<std::string> labels;
    for (auto& c : containerList)
        labels.push_back(c.type + " - Size " + c.size);

    std::vector<const char*> items;
    for (auto& s : labels)
        items.push_back(s.c_str());

    ImGui::Combo("##Container", &containerIndex, items.data(), (int)items.size());

    // ======================================================
    // คำนวณราคาตรงนี้
    // ======================================================
    const Container& selected = containerList[containerIndex];
    const char* selectedOccasion = occasionItems[occasionIndex];
    BudgetRange actualPrice = calculateActualBudget(selectedOccasion, selected);

    ImGui::Text("Flowers: %d - %d | Base Price: %d",
        selected.minF, selected.maxF, selected.basePrice);

    ImGui::Text("Estimated Price: %d - %d",
        actualPrice.min, actualPrice.max);

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    auto updateSelection = [&]() {
        selection.occasion = occasionItems[occasionIndex]; // เก็บชื่อโอกาส
        selection.containerType = selected.type;           // เก็บชนิดภาชนะ
        selection.containerSize = selected.size;           // เก็บขนาด
    };

    ImGui::SetCursorPosX(35); 
    if (ImGui::Button("Custom Mode", {200,45})) {
        updateSelection(); // <- เรียกฟังก์ชันเก็บค่า
        state = AppState::CUSTOM_MODE;
    }

    ImGui::SameLine();

    if (ImGui::Button("Random Mode", {200,45})) {
        updateSelection(); // <- เรียกฟังก์ชันเก็บค่า
        state = AppState::RANDOM_MODE;
    }

    ImGui::Spacing();
    ImGui::SetCursorPosX((ImGui::GetWindowSize().x - 120) * 0.5f);
    if (ImGui::Button("Exit", {120,35}))
        state = AppState::EXIT;

    ImGui::PopFont();
    ImGui::End();
}