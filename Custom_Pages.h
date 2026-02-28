#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/System/Clock.hpp>
#include <iostream>
#include <optional>
#include <vector>
#include <string>

#include "config.h"
#include "font_manager.h"
#include "ui_mainmenu.h"
#include "data_models.h"

void Selectyourpieces();
void Summary();

void Custom_Pages(std::vector<Flower>& flowerList,std::vector<Flower>& selectedFlowers,AppState& state){
    static int currentPage = 0; 
    
        Selectyourpieces();

        ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.05f, 0.10f, 0.20f, 1.0f)); 
        ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.10f, 0.15f, 0.30f, 1.0f)); 
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 15.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.85f, 0.45f, 0.55f, 1.0f)); //สีพื้นหลัง
        ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Always); 
        ImGui::SetNextWindowSize(ImVec2(WINDOW_WIDTH-40, WINDOW_HEIGHT-40), ImGuiCond_Always);
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); 
        ImGui::Begin("Custom Page", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
        ImGui::PopStyleColor(1); 

        if (currentPage == 0){
            ImGui::PushFont(FONT_TITLE);    
            Selectyourpieces();
            ImGui::PopFont();
            
            float footerHeight = 70.0f; 
            ImGui::BeginChild("FlowerScrollZone", ImVec2(0, -footerHeight), false, ImGuiWindowFlags_None);
            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            ImVec2 windowPos = ImGui::GetCursorScreenPos();

            const int columns = 6;
            const float spacing = 20.0f;     
            const float boxSize = 150.0f;    // เพิ่มขนาดกล่องนิดหน่อยเพื่อให้มีที่วางข้อความ
            const float imgSize = 90.0f;     // ขนาดรูปภาพ

        for (int i = 0; i < (int)flowerList.size(); i++) {
            int row = i / columns;
            int col = i % columns;

            ImVec2 boxPos = ImVec2(windowPos.x + (col * (boxSize + spacing)), 
                                windowPos.y + (row * (boxSize + spacing)));
            ImVec2 boxMax = ImVec2(boxPos.x + boxSize, boxPos.y + boxSize);

            // --- 1. วาดพื้นหลังกล่อง ---
            draw_list->AddRectFilled(boxPos, boxMax, ImColor(255, 255, 255, 150), 12.0f); // กล่องสีขาว
            draw_list->AddRect(boxPos, boxMax, ImColor(255, 255, 255, 255), 12.0f, 0, 1.0f); // ขอบ

            // --- 2. วาดรูปดอกไม้ (เขยิบขึ้นบนนิดนึง) ---
            if (i < (int)flowerList.size()) {
    
            // 2. คำนวณตำแหน่งเริ่มต้น (มุมซ้ายบน)
            ImVec2 imgPos = ImVec2(boxPos.x + (boxSize - imgSize) / 2.0f, boxPos.y + 15.0f);
            
            // 3. คำนวณตำแหน่งสิ้นสุด (มุมขวาล่าง) **ต้องเพิ่มบรรทัดนี้**
            ImVec2 imgMax = ImVec2(imgPos.x + imgSize, imgPos.y + imgSize);
            
            // 4. วาดรูปโดยดึงจาก flowerList ตรงๆ
            draw_list->AddImage((void*)(intptr_t)flowerList[i].texture.getNativeHandle(), imgPos, imgMax);
        }

            // --- 3. แสดงชื่อและราคาบนสี่เหลี่ยมเลย ---
            // วาดชื่อดอกไม้ (กลาง)
            std::string nameText = flowerList[i].name;
            float nameWidth = ImGui::CalcTextSize(nameText.c_str()).x;
            draw_list->AddText(ImVec2(boxPos.x + (boxSize - nameWidth) / 2.0f, boxPos.y + 110.0f), 
                            ImColor(0, 0, 0, 255), nameText.c_str());

            // วาดราคา (ล่าง) - ใช้สีเขียวให้ดูโดดเด่น
            std::string priceText = std::to_string(flowerList[i].price) + " THB";
            float priceWidth = ImGui::CalcTextSize(priceText.c_str()).x;
            draw_list->AddText(ImVec2(boxPos.x + (boxSize - priceWidth) / 2.0f, boxPos.y + 128.0f), 
                            ImColor(0, 0, 0, 255), priceText.c_str());

            // --- 4. ระบบ Interaction & Tooltip สำหรับความหมาย ---
            ImGui::SetCursorScreenPos(boxPos);
            ImGui::PushID(i);
            ImGui::InvisibleButton("FlowerBtn", ImVec2(boxSize, boxSize));

            if (ImGui::IsItemHovered()) {
                // ไฮไลต์ขอบเมื่อเอาเมาส์วาง
                draw_list->AddRect(boxPos, boxMax, ImColor(255, 215, 0, 255), 12.0f, 0, 2.0f);

                // แสดงเฉพาะความหมายใน Tooltip
                ImGui::BeginTooltip();
                ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.8f, 1.0f), "Meaning:");
                ImGui::TextWrapped("%s", flowerList[i].meaning.c_str()); // ใช้ Wrapped เผื่อความหมายยาว
                ImGui::EndTooltip();
            }
            ImGui::SetItemAllowOverlap();//ให้คลิกปุ่มบนแถบได้แม้ตำแหน่งทับซ้อนกัน
            ImVec2 winPos = ImGui::GetWindowPos();
            ImVec2 winSize = ImGui::GetWindowSize();
            float barHeight = 60.0f; // ความสูงของแถบสีขาว
            ImVec2 barMin = ImVec2(winPos.x, winPos.y + winSize.y - barHeight);
            bool mouseOnBar = (ImGui::GetIO().MousePos.y >= barMin.y);
            if (ImGui::IsItemClicked() && ImGui::GetIO().MousePos.y < barMin.y) {
                selectedFlowers.push_back(flowerList[i]);
                std::cout << "Added to cart: " << flowerList[i].name << std::endl;
                
            }
            
            ImGui::PopID();
            }
             // บังคับความสูงเพื่อให้เลื่อนได้
            int totalRows = ((int)flowerList.size() + 5) / 6; 
            float totalHeight = totalRows * (boxSize + spacing);
            ImGui::SetCursorPosY(totalHeight + 20.0f);

            ImGui::EndChild();
            ImGui::Dummy(ImVec2(0.0f, 25.0f));//เพิ่มพื้นที่เลื่อนลงหลังแสดงดอกไม้
            ImVec2 winPos = ImGui::GetWindowPos();
            ImVec2 winSize = ImGui::GetWindowSize();
            ImDrawList* drawList = ImGui::GetWindowDrawList();

            // --- 2. ตั้งค่าขนาดแถบด้านล่าง ---
            float barHeight = 60.0f; // ความสูงของแถบสีขาว
            ImVec2 barMin = ImVec2(winPos.x, winPos.y + winSize.y - barHeight);
            ImVec2 barMax = ImVec2(winPos.x + winSize.x, winPos.y + winSize.y);

            // --- 3. วาดแถบสีขาวทึบ (Background Bar) ---
            // ใช้สีขาว (255, 255, 255) และความมนเฉพาะมุมล่าง (Rounding) ให้เข้ากับหน้าต่าง
            drawList->AddRectFilled(barMin, barMax, ImColor(255, 255, 255, 255), 15.0f, ImDrawFlags_RoundCornersBottom);
            // เพิ่มเส้นขอบบนของแถบนิดนึงให้ดูแยกส่วนชัดเจน
            drawList->AddLine(ImVec2(barMin.x, barMin.y), ImVec2(barMax.x, barMin.y), ImColor(200, 200, 200, 255), 1.5f);

            // --- 4. วาดปุ่มลงบนแถบที่สร้างไว้ ---
            float btnW = 110.0f;
            float btnH = 45.0f;
            float padX = 30.0f;
            float gap = 20.0f;

            // คำนวณพิกัดปุ่มให้อยู่กึ่งกลางแนวตั้งของแถบสีขาว
            float btnY = barMin.y + (barHeight - btnH) / 2.0f;
            float nextX = winPos.x + winSize.x - btnW - padX;
            float backX = nextX - btnW - gap;

            
            // --- 1. เตรียมข้อมูล ---
            int totalP = 0;
            for(const auto& f : selectedFlowers) totalP += f.price;

            // --- 2. อ้างอิงพิกัดจากแถบสีขาว (barMin ที่เราคำนวณไว้ก่อนหน้านี้) ---
            // สมมติว่า barMin.y คือจุดเริ่มต้นของแถบขาวด้านล่าง
            float textY = barMin.y + (barHeight - ImGui::GetTextLineHeight()) / 2.0f; 
            float leftPadding = 30.0f; // ระยะห่างจากขอบซ้ายของหน้าต่าง

            // --- 3. วาดจำนวนดอกไม้ (ฝั่งซ้าย) ---
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.85f, 0.45f, 0.55f, 1.0f)); // สีชมพูพาสเทลเข้ม
            std::string txtItems = "Selected: " + std::to_string(selectedFlowers.size()) + " pieces";
            ImGui::SetCursorScreenPos(ImVec2(winPos.x + leftPadding, textY));
            ImGui::Text("%s", txtItems.c_str());

            // --- 4. วาดราคา (ถัดไปทางขวา) ---
            // นายสามารถปรับระยะห่างระหว่าง "ชิ้น" กับ "ราคา" ได้ที่ค่า gap
            float gapBetween = 40.0f; 
            float priceX = winPos.x + leftPadding + ImGui::CalcTextSize(txtItems.c_str()).x + gapBetween;

            ImGui::SetCursorScreenPos(ImVec2(priceX, textY));
            ImGui::Text("|  Total: %d THB", totalP); // ใส่เส้นแบ่งนิดนึงให้ดูสวย
            // ปุ่ม Back
            ImGui::SetCursorScreenPos(ImVec2(backX, btnY));
            if (ImGui::Button("<< Back", ImVec2(btnW, btnH))) {
                state = AppState::MAIN_MENU;
            }

            // ปุ่ม Next
            ImGui::SameLine();
            ImGui::SetCursorScreenPos(ImVec2(nextX, btnY));
            if (ImGui::Button("Next >>", ImVec2(btnW, btnH))) {
                currentPage = 1;
            }
            ImGui::PopStyleColor(1);

        }
       
            
        
        else if (currentPage == 1) {
            
        ImGui::PushFont(FONT_TITLE);
        Summary();
        ImGui::PopFont();
      
        ImGui::Text("Your Selected Flowers:");
        for (const auto& flower : selectedFlowers) {
            ImGui::BulletText("%s - %d THB", flower.name.c_str(), flower.price);
        }

        // ใช้คำสั่ง Text เพื่อดึงขนาดของ vector มาแสดงผล
        ImGui::Text("Total Selected: %d pieces", (int)selectedFlowers.size());
        
    
        const float marginSide = 30.0f;    // ห่างจากขอบขวา 20
        const float marginBottom = 5.0f;  // ห่างจากขอบล่าง 20
        const float spacing = 20.0f;       // ระยะห่างระหว่างปุ่ม Back กับ Next
        const float bWidth = 100.0f;
        const float bHeight = 40.0f;

        // 1. คำนวณตำแหน่ง Y (ห่างจากขอบล่างของหน้าต่าง)
        float posY = ImGui::GetWindowSize().y - bHeight - marginBottom;
        ImGui::SetCursorPosY(posY);

        // 2. คำนวณตำแหน่ง X สำหรับปุ่มแรก (Back) 
        // สูตร: ความกว้างหน้าต่าง - (ความกว้าง 2 ปุ่ม) - (ระยะห่างระหว่างปุ่ม) - (ขอบขวา)
        float firstBtnPosX = ImGui::GetWindowSize().x - (bWidth * 2) - spacing - marginSide;

        // 3. วาดปุ่ม Back
        ImGui::SetCursorPosX(firstBtnPosX);
        if (ImGui::Button("<< Back", ImVec2(bWidth, bHeight))) {
            currentPage = 0;
        }

        // 4. วาดปุ่ม Next ไว้ข้างกัน (ห่างจากขอบขวา 20 พอดี)
        ImGui::SameLine();
        float secondBtnPosX = ImGui::GetWindowSize().x - bWidth - marginSide;
        ImGui::SetCursorPosX(secondBtnPosX);

        if (ImGui::Button("Next >>", ImVec2(bWidth, bHeight))) {
            state = AppState::SUMMARY;
        }
    }


        ImGui::PopStyleColor(3); 
        ImGui::PopStyleVar(2); 
        ImGui::End(); 


}

void Selectyourpieces() {
    float textWidth = ImGui::CalcTextSize("Select your flower").x;
    float centerX = (WINDOW_WIDTH*0.9f - textWidth*1.0f) * 0.5f; 
    ImGui::SetCursorPosX(centerX); 
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.1f, 0.1f, 0.44f, 1.0f)); 
    ImGui::SetWindowFontScale(1.0f); 
    ImGui::Text("Select your flower");
    ImGui::SetWindowFontScale(1.0f); 
    ImGui::PopStyleColor(1); 
}
void Summary() {
    float textWidth = ImGui::CalcTextSize("Summary").x;
    float centerX = (WINDOW_WIDTH*0.9f - textWidth*1.0f) * 0.5f; 
    ImGui::SetCursorPosX(centerX); 
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.1f, 0.1f, 0.44f, 1.0f)); 
    ImGui::SetWindowFontScale(1.0f); 
    ImGui::Text("Summary");
    ImGui::SetWindowFontScale(1.0f); 
    ImGui::PopStyleColor(1); 
}
