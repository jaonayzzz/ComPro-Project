#ifndef FOR_TESTING_H
#define FOR_TESTING_H

#include <imgui.h>
#include <vector>
#include <algorithm>
#include "Config.h"
#include "DataModels.h"


void ShowContainerFullPopup(bool& show, int& currentPage, Container& selectedCont, std::vector<Container>& containerList, UserSelection& selection) {
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.25f, 0.35f, 0.55f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.20f, 0.30f, 0.50f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

    if (ImGui::BeginPopupModal("ContainerFullOptions", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        float buttonWidth = 180.0f;
        float buttonLength1 = 30.0f;
        float buttonLength2 = 20.0f;

        ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "Container is Full!");
        // ใช้ selectedCont.maxF แทน max ที่อาจจะเป็นตัวแปรลอย
        ImGui::Text("You have reached the maximum limit of %d flowers.", selectedCont.maxF); 
        ImGui::Separator();

        if (ImGui::Button("Complete Selection", ImVec2(buttonWidth, buttonLength1))) {
            currentPage = 1; 
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        // Logic เช็กการอัปเกรด
        bool hasLarger = false;
        Container* largerContainer = nullptr;
        for (auto& c : containerList) {
            if (c.type == selectedCont.type && c.maxF > selectedCont.maxF) {
                if (largerContainer == nullptr || c.maxF < largerContainer->maxF) {
                    largerContainer = &c;
                    hasLarger = true;
                }
            }
        }

        if (!hasLarger) ImGui::BeginDisabled();
        if (ImGui::Button("Upgrade Size", ImVec2(buttonWidth, buttonLength1))) {
            if (largerContainer != nullptr) {
                selectedCont = *largerContainer; // อัปเดตตัวแปรในหน้านั้นด้วย
                selection.containerType = largerContainer->type;
                selection.containerSize = largerContainer->size;
                ImGui::CloseCurrentPopup();
            }
        }
        if (!hasLarger) ImGui::EndDisabled();

        float posX = (ImGui::GetWindowSize().x - buttonWidth) * 0.5f;
        ImGui::SetCursorPosX(posX);
        if (ImGui::Button("Close", ImVec2(buttonWidth, buttonLength2))) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
    ImGui::PopStyleColor(3);
}
void ShowBackWarningPopup(bool& showBackWarning, std::vector<Flower>& selectedFlowers, AppState& state) {
    if (showBackWarning) {
        ImGui::OpenPopup("BackWarningPopup");
        showBackWarning = false; // รีเซ็ตเพื่อไม่ให้ OpenPopup รัวๆ ทุกเฟรม
    }
    float buttonWidth = 180.0f;
    float buttonLength = 30.0f;
    // ตั้งค่าตำแหน่ง Popup ให้อยู่กลางจอ
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal("BackWarningPopup", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "Warning!");
        ImGui::Text("all your selected flowers will be cleared.");
        ImGui::Text("Are you sure?");
        ImGui::Separator();

        if (ImGui::Button("Yes, Clear and Back", ImVec2(buttonWidth, buttonLength))) {
            selectedFlowers.clear();      // ล้างข้อมูลดอกไม้
            state = AppState::MAIN_MENU;  // กลับหน้าหลัก
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(buttonWidth, buttonLength))) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}
inline void SendYourFeelings() {
    float textWidth = ImGui::CalcTextSize("Send Your Feelings").x;
    float centerX = (WINDOW_WIDTH*0.9f - textWidth*1.0f) * 0.5f; 
    ImGui::SetCursorPosX(centerX); 
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.1f, 0.1f, 0.44f, 1.0f)); 
    ImGui::SetWindowFontScale(1.0f); 
    ImGui::Text("Send Your Feelings");
    ImGui::SetWindowFontScale(1.0f); 
    ImGui::PopStyleColor(1); 
}
inline void OrderSummary() {
    float textWidth = ImGui::CalcTextSize("Order Summary").x;
    float centerX = (WINDOW_WIDTH*0.9f - textWidth*1.0f) * 0.5f; 
    ImGui::SetCursorPosX(centerX); 
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.1f, 0.1f, 0.44f, 1.0f)); 
    ImGui::SetWindowFontScale(1.0f); 
    ImGui::Text("Order Summary");
    ImGui::SetWindowFontScale(1.0f); 
    ImGui::PopStyleColor(1); 
}

#endif