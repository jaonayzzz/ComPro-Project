// Summary_Page.cpp Test
#include <imgui.h>
#include "AppState.h"
#include "DataModels.h"
#include "render/FlowerRender.h"
#include "Config.h"
#include <map>

static std::map<std::string, sf::Texture> summaryCache;

static sf::Texture& getSummaryTexture(const std::string& path) {
    if (summaryCache.find(path) != summaryCache.end())
        return summaryCache[path];

    summaryCache[path].loadFromFile(path);
    summaryCache[path].setSmooth(true);
    return summaryCache[path];
}

void drawSummaryPage(sf::RenderWindow& window,AppState& state, UserSelection& selection,const std::vector<sf::Texture>& flowerTextures) {
    
    if (selection.source != EntrySource::PRESET) {
    renderBouquet(window, selection, flowerTextures);
}
    else {
    sf::Texture& tex = getSummaryTexture(selection.presetImagePath);

    if (tex.getNativeHandle() != 0) {
        sf::Sprite sprite(tex);

        float scaleX = 600.f / tex.getSize().x;
        float scaleY = 600.f / tex.getSize().y;
        float scale = std::min(scaleX, scaleY);

        sprite.setScale({scale, scale});
        sf::Vector2f canvasCenter(WINDOW_WIDTH * 0.55f, WINDOW_HEIGHT / 3.6f);
        sprite.setOrigin({
        sprite.getGlobalBounds().size.x / 2.f,
        sprite.getGlobalBounds().size.y / 2.f
        });
        sprite.setPosition(canvasCenter);

        window.draw(sprite);
    }
}

    ImGui::SetNextWindowPos({20.f, 20.f}, ImGuiCond_Always); 
    ImGui::SetNextWindowSize({350.f, (float)WINDOW_HEIGHT - 40.f});

    ImGui::Begin("Order Summary", nullptr, ImGuiWindowFlags_NoResize);
    ImGui::PushFont(FONT_BODY);
    ImGui::Text("Your Bouquet Summary");
    ImGui::Separator();
    ImGui::Text("Occasion: %s", selection.occasion.c_str());
    ImGui::Text("Container: %s (%s)", selection.containerType.c_str(), selection.containerSize.c_str());
    ImGui::Separator();

    if (selection.source == EntrySource::PRESET)
{
    ImGui::Text("Preset: %s", selection.presetName.c_str());
    ImGui::Separator();
    for (const auto& name : selection.flowers)
    {
        ImGui::BulletText("%s", name.c_str());
    }
    ImGui::Text("Total Price: %d THB", selection.totalAmount);
}
else
{
    int totalFlowerPrice = 0;
    // แสดงรายการดอกไม้ที่เลือกไว้ใน selectedFlowers
    for (const auto& f : selection.selectedFlowers) {
        ImGui::BulletText("%s - %d THB", f.name.c_str(), f.price);
        totalFlowerPrice += f.price;
    }
    ImGui::Separator();
    ImGui::Text("Total Price: %d THB", totalFlowerPrice);
}
    
    if (ImGui::Button("Back", {120, 40})) {
        selection.selectedFlowers.clear(); // ล้างข้อมูลเก่า
        state = selection.getReturnState();
    }
    if (ImGui::Button("Next", {120, 40})) { 
        state = AppState::CARD;
    }
    ImGui::PopFont();
    ImGui::End();
}