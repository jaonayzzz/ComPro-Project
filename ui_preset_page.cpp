#include "ui_preset_page.h"
#include "data_models.h"
#include "config.h"
#include "font_manager.h"

#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>
#include <map>
#include <string>

// Texture Cache สำหรับการโหลดรูปอัตโนมัติ
static std::map<std::string, sf::Texture> pageCache;

static sf::Texture& getCachedTexture(const std::string& path) {
    if (pageCache.find(path) != pageCache.end()) {
        return pageCache[path];
    }
    if (!pageCache[path].loadFromFile(path)) {
        printf("Error: Cannot load texture at %s\n", path.c_str());
    }
    pageCache[path].setSmooth(true);
    return pageCache[path];
}

void drawPresetPage(
    sf::RenderWindow& window,
    AppState& state,
    UserSelection& selection)
{
    static sf::Texture bgTexture;
    static sf::Sprite* bgSprite = nullptr;
    static bool bgLoaded = false;

    if (!bgLoaded) {
        if (bgTexture.loadFromFile("../picture/Background.png")) {
            bgSprite = new sf::Sprite(bgTexture);
            bgSprite->setScale({
                (float)WINDOW_WIDTH / bgTexture.getSize().x,
                (float)WINDOW_HEIGHT / bgTexture.getSize().y
            });
        }
        bgLoaded = true;
    }

    if (bgSprite) {
        bgSprite->setColor(sf::Color(255, 255, 255, 180));
        window.draw(*bgSprite);
    }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(30, 25));
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 18.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(20, 5));

    ImGui::SetNextWindowPos(ImVec2(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize({950, 640});

    ImGui::Begin("PresetMainContainer", nullptr, 
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | 
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);

    // --- Header Title ---
    ImGui::PushFont(FONT_TITLE);
    float titleWidth = ImGui::CalcTextSize("Special Selection").x;
    ImGui::SetCursorPosX((ImGui::GetWindowSize().x - titleWidth) * 0.5f);
    ImGui::SetCursorPosY(35);
    ImGui::TextColored(COLOR_TEXT, "Special Selection");
    ImGui::PopFont();

    ImGui::SetCursorPosY(90);
    ImGui::Separator();
    ImGui::Spacing();

    if (ImGui::BeginChild("InnerScrollZone", ImVec2(0, -90), false, ImGuiWindowFlags_AlwaysVerticalScrollbar)) 
    {
        float contentWidth = ImGui::GetContentRegionAvail().x;
        int columns = 3;
        float spacing = ImGui::GetStyle().ItemSpacing.x;
        float cardWidth = (contentWidth - (spacing * (columns - 1))) / columns;
        float cardHeight = 390.0f; 

        for (int i = 0; i < (int)developerPresets.size(); i++) 
        {
            auto& preset = developerPresets[i]; // ตัวแปรหลักที่ใช้ในการ์ดนี้
            if (i % columns != 0) ImGui::SameLine();

            ImGui::PushID(i);
            ImGui::BeginGroup();

            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.88f, 0.95f, 1.00f, 0.90f));
            if (ImGui::BeginChild("CardFrame", ImVec2(cardWidth, cardHeight), true, 
                ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse)) 
            {
                // --- รูปภาพ (Aspect Ratio คงที่) ---
                sf::Texture& flowerTex = getCachedTexture(preset.imagePath);
                if (flowerTex.getNativeHandle() != 0) {
                    float texW = (float)flowerTex.getSize().x;
                    float texH = (float)flowerTex.getSize().y;
                    float displayW = cardWidth - 40.0f;
                    float displayH = displayW * (texH / texW); 

                    if (displayH > 200.0f) {
                        displayH = 200.0f;
                        displayW = displayH * (texW / texH);
                    }

                    ImGui::SetCursorPosX((cardWidth - displayW) * 0.5f);
                    ImGui::SetCursorPosY(20);
                    ImGui::Image(flowerTex, sf::Vector2f(displayW, displayH));
                }

                // --- ข้อความ ---
                ImGui::SetCursorPosY(235);
                
                ImGui::PushFont(FONT_BODY);
                float nameWidth = ImGui::CalcTextSize(preset.name.c_str()).x;
                ImGui::SetCursorPosX((cardWidth - nameWidth) * 0.5f);
                ImGui::TextColored(COLOR_TEXT, "%s", preset.name.c_str());

                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.45f, 0.45f, 0.45f, 1.0f));
                float occWidth = ImGui::CalcTextSize(preset.occasion.c_str()).x;
                ImGui::SetCursorPosX((cardWidth - occWidth) * 0.5f);
                ImGui::Text("%s", preset.occasion.c_str());
                ImGui::PopStyleColor();

                ImGui::SetCursorPosY(cardHeight - 65); 
                ImGui::PushStyleColor(ImGuiCol_Button, COLOR_BUTTON);
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, COLOR_BUTTON_HOVER);
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, COLOR_BUTTON_ACTIVE);
                
                std::string priceText = std::to_string(preset.price) + " THB";
                ImGui::SetCursorPosX(20);
                if (ImGui::Button(priceText.c_str(), ImVec2(cardWidth - 40, 45))) {
                    selection.source = EntrySource::PRESET;
                    selection.presetName = preset.name;
                    selection.presetImagePath = preset.imagePath;
                    selection.occasion = preset.occasion;
                    selection.containerType = preset.containerType;
                    selection.containerSize = preset.containerSize;
                    selection.flowers = preset.flowers;
                    selection.totalAmount = preset.price;
                    selection.source = EntrySource::PRESET;
                    state = AppState::SUMMARY;
                }
                ImGui::PopStyleColor(3);
                ImGui::PopFont();
            }
            ImGui::EndChild();
            ImGui::PopStyleColor();
            ImGui::EndGroup();
            ImGui::PopID();
        }
    }
    ImGui::EndChild();

    ImGui::Separator();
    ImGui::SetCursorPosY(ImGui::GetWindowSize().y - 65);
    ImGui::SetCursorPosX((ImGui::GetWindowSize().x - 150) * 0.5f);
    
    ImGui::PushFont(FONT_BODY);
    if (ImGui::Button("Back", ImVec2(150, 45))) {
        state = AppState::MAIN_MENU;
    }
    ImGui::PopFont();

    ImGui::End();

    // คืนค่าสไตล์กลับเป็นปกติ เพื่อไม่ให้กระทบหน้า Main Menu
    ImGui::PopStyleVar(4);
}