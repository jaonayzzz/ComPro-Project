// Summary_Page.cpp Test
#include <imgui.h>
#include "app_state.h"
#include "data_models.h"
#include "config.h"

void drawSummaryPage(AppState& state, UserSelection& selection) {
    ImGui::SetNextWindowPos({WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * 0.5f}, ImGuiCond_Always, {0.5f, 0.5f});
    ImGui::SetNextWindowSize({500, 600});

    ImGui::Begin("Order Summary", nullptr, ImGuiWindowFlags_NoResize);

    ImGui::Text("Your Bouquet Summary");
    ImGui::Separator();

    int totalFlowerPrice = 0;
    // แสดงรายการดอกไม้ที่เลือกไว้ใน selectedFlowers
    for (const auto& f : selection.selectedFlowers) {
        ImGui::BulletText("%s - %d THB", f.name.c_str(), f.price);
        totalFlowerPrice += f.price;
    }

    ImGui::Separator();
    ImGui::Text("Total Price: %d THB", totalFlowerPrice);
    
    if (ImGui::Button("Back to Menu", {120, 40})) {
        selection.selectedFlowers.clear(); // ล้างข้อมูลเก่า
        state = AppState::MAIN_MENU;
    }

    ImGui::End();
}