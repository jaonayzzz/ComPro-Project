#include "ui/MainMenu.h"
#include "DataModels.h"
#include "core/FontManager.h"
#include "Config.h"

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
    static sf::Texture menuBgTexture;
    static sf::Sprite* menuBgSprite = nullptr;
    static bool bgLoaded = false;

    if (!bgLoaded)
    {
        menuBgTexture.loadFromFile("../assets/picture/Background.png");
        menuBgSprite = new sf::Sprite(menuBgTexture);
        menuBgSprite->setScale(sf::Vector2f(
            (float)WINDOW_WIDTH / menuBgTexture.getSize().x,
            (float)WINDOW_HEIGHT / menuBgTexture.getSize().y
        ));
        bgLoaded = true;
    }

    menuBgSprite->setColor(sf::Color(255,255,255,180));
    window.draw(*menuBgSprite);

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

    style.FrameBorderSize  = 1.5f;
    style.WindowBorderSize = 0.f;
    style.WindowRounding = ROUND_WINDOW;
    style.FrameRounding = ROUND_FRAME;
    style.PopupRounding = ROUND_POPUP;

    // คำนวณจุดกึ่งกลางจากค่า WINDOW_WIDTH และ WINDOW_HEIGHT
    ImGui::SetNextWindowPos(
        ImVec2((float)WINDOW_WIDTH / 2.f, (float)WINDOW_HEIGHT / 2.f), 
        ImGuiCond_Always, 
        ImVec2(0.5f, 0.5f)
    );
    ImGui::SetNextWindowSize({480, 460});

    ImGui::Begin("Chonampay Florist", nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoMove
    );

    ImGui::PushFont(FONT_TITLE);
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, anim);
    
    // จัดตัวอักษรให้อยู่กึ่งกลางหน้าต่าง ImGui
    float titleWidth = ImGui::CalcTextSize("Chonampay Florist").x;
    ImGui::SetCursorPosX((ImGui::GetWindowSize().x - titleWidth) * 0.5f);
    
    ImGui::Text("Chonampay Florist");
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

    // คำนวณราคา
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
        updateSelection();
        state = AppState::CUSTOM_MODE;
    }

    ImGui::SameLine();

    if (ImGui::Button("Random Mode", {200,45})) {
        updateSelection();
        state = AppState::RANDOM_MODE;
    }
    
    ImGui::Spacing();
    float presetBtnWidth = 200.0f; 
    ImGui::SetCursorPosX((ImGui::GetWindowSize().x - presetBtnWidth) * 0.5f);
    if (ImGui::Button("Preset Mode", {presetBtnWidth, 45})) {
        state = AppState::PRESET_PAGE;
    }

    ImGui::Spacing();
    ImGui::SetCursorPosX((ImGui::GetWindowSize().x - 120) * 0.5f);
    if (ImGui::Button("Exit", {120,35}))
        state = AppState::EXIT;

    ImGui::PopFont();
    ImGui::End();
}