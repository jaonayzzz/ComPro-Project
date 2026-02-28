// Summary_Page.cpp Test
#include <imgui.h>
#include "app_state.h"
#include "data_models.h"
#include "FlowerRender.h"
#include "config.h"

void drawSummaryPage(sf::RenderWindow& window,AppState& state, UserSelection& selection,const std::vector<sf::Texture>& flowerTextures) {
    
    renderBouquet(window, selection, flowerTextures);

    ImGui::SetNextWindowPos({20.f, 20.f}, ImGuiCond_Always); 
    ImGui::SetNextWindowSize({350.f, (float)WINDOW_HEIGHT - 40.f});

    ImGui::Begin("Order Summary", nullptr, ImGuiWindowFlags_NoResize);
    ImGui::PushFont(FONT_BODY);
    ImGui::Text("Your Bouquet Summary");
    ImGui::Separator();
    ImGui::Text("Occasion: %s", selection.occasion.c_str());
    ImGui::Text("Container: %s (%s)", selection.containerType.c_str(), selection.containerSize.c_str());
    ImGui::Separator();

    int totalFlowerPrice = 0;
    // แสดงรายการดอกไม้ที่เลือกไว้ใน selectedFlowers
    for (const auto& f : selection.selectedFlowers) {
        ImGui::BulletText("%s - %d THB", f.name.c_str(), f.price);
        totalFlowerPrice += f.price;
    }

    ImGui::Separator();
    ImGui::Text("Total Price: %d THB", totalFlowerPrice);
    
    if (ImGui::Button("Back", {120, 40})) {
        selection.selectedFlowers.clear(); // ล้างข้อมูลเก่า
        state = AppState::RANDOM_MODE;
    }
    if (ImGui::Button("Next", {120, 40})) { 
        state = AppState::CARD;
    }
    ImGui::PopFont();
    ImGui::End();
}