#ifndef FOR_TESTING_H
#define FOR_TESTING_H

#include <imgui.h>
#include <vector>
#include <algorithm>
#include "Config.h"
#include "DataModels.h"

    static int currentPage = 0; 
    static float notificationTimer = 0.0f; // 0 คือไม่แสดง, > 0 คือกำลังแสดง
    float displayDuration = 2.0f;          // ให้แสดงค้างไว้ 2 วินาที
    static bool showWarning = false; // สำหรับเปิด/ปิด Modal เตือน
    static bool openFullPopup = false;
    static bool showBackWarning = false;
    std::map<std::string, int> flowerCounts;
    std::map<std::string, int> flowerPrices;

    const int columns = 6;
    const float spacing = 20.0f;           //ระยะห่างระหว่างกล่อง เท่ากันทั้ง row และ column
    const float boxSize = 150.0f;          // เพิ่มขนาดกล่องนิดหน่อยเพื่อให้มีที่วางข้อความ
    const float imgSize = 0.6*boxSize;     //ขนาดรูปภาพ

    float footerHeight = barHeight+10.0f; //เว้นพื้นที่ scrollzone ไม่ให้ชนแถบด้านล่าง

    float barHeight = 60.0f; // ความสูงของแถบสีขาว

    // วาดปุ่มลงบนแถบที่สร้างไว้ 
    float btnW = 110.0f;
    float btnH = 45.0f;
    float padX = 30.0f;
    float gap = 20.0f;

    int totalP = 0;
    float gapBetween = 40.0f; //ระยะห่างระหว่าง "Selected" กับ "Total" 

    
    float topPadding = 80.0f;    // ระยะจากด้านบน (เผื่อ Title)
    float bottomPadding = 60.0f; // ระยะห่างจากขอบล่าง 
    float textInsidePadding = 15.0f; //

void ShowContainerFullPopup(bool& show, int& currentPage, Container& selectedCont, std::vector<Container>& containerList, UserSelection& selection) {
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.25f, 0.35f, 0.55f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.20f, 0.30f, 0.50f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

    //เซตตำแหน่ง pop up
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    // สั่งให้ Window ถัดไป (ซึ่งคือ Popup) วางตัวตรงกลาง
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));// พิกัดตามcenterและใช้ Pivot (0.5f, 0.5f) เพื่อให้จุดกึ่งกลางของ Popup ทับกับจุด center พอดี

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

inline void PrintSendYourFeelings() {
    ImGui::PushFont(FONT_TITLE);    
    float textWidth = ImGui::CalcTextSize("Send Your Feelings").x;
    float centerX = (WINDOW_WIDTH*0.9f - textWidth*1.0f) * 0.5f; 
    ImGui::SetCursorPosX(centerX); 
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.1f, 0.1f, 0.44f, 1.0f)); 
    ImGui::SetWindowFontScale(1.0f); 
    ImGui::Text("Send Your Feelings");
    ImGui::SetWindowFontScale(1.0f); 
    ImGui::PopStyleColor(1); 
    ImGui::PopFont();
}

inline void OrderSummary() {
    ImGui::PushFont(FONT_TITLE);  
    float textWidth = ImGui::CalcTextSize("Order Summary").x;
    float centerX = (WINDOW_WIDTH*0.9f - textWidth*1.0f) * 0.5f; 
    ImGui::SetCursorPosX(centerX); 
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.1f, 0.1f, 0.44f, 1.0f)); 
    ImGui::SetWindowFontScale(1.0f); 
    ImGui::Text("Order Summary");
    ImGui::SetWindowFontScale(1.0f); 
    ImGui::PopStyleColor(1); 
    ImGui::PopFont();
}

#endif