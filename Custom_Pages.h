#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/System/Clock.hpp>
#include <iostream>
#include <optional>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#ifndef CUSTOM_PAGES_H
#define CUSTOM_PAGES_H

#include "config.h"
#include "font_manager.h"
#include "ui_mainmenu.h"
#include "data_models.h"
#include "FlowerRenderer.h"
#include "for_testing.h"

void ShowBackWarningPopup(bool& showBackWarning, std::vector<Flower>& selectedFlowers, AppState& state);
void ShowContainerFullPopup(bool& show, int& currentPage, Container& selectedCont, std::vector<Container>& containerList, UserSelection& selection);
inline void SendYourFeelings();
inline void OrderSummary();

inline void Custom_Pages(std::vector<Flower>& flowerList,std::vector<Flower>& selectedFlowers,AppState& state,UserSelection& selection){
    static int currentPage = 0; 
    static float notificationTimer = 0.0f; // 0 คือไม่แสดง, > 0 คือกำลังแสดง
    float displayDuration = 2.0f;          // ให้แสดงค้างไว้ 2 วินาที
    static bool showWarning = false; // สำหรับเปิด/ปิด Modal เตือน
    static bool openFullPopup = false;
    static bool showBackWarning = false;
    std::map<std::string, int> flowerCounts;
    std::map<std::string, int> flowerPrices;

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
        // --- 1. ค้นหาข้อมูล Container ---
        auto it = std::find_if(containerList.begin(), containerList.end(), [&](const Container& c) {
            return c.type == selection.containerType && c.size == selection.containerSize;
        });

        Container selectedCont = (it != containerList.end()) ? *it : containerList[0];

        // --- 2. เปลี่ยนชื่อตัวแปรให้ตรงกับใน struct Container ของนาย ---
        // ลองเปลี่ยนจาก minF เป็น minFlowers และ maxF เป็น maxFlowers
        int min = selectedCont.minF;; // <--- แก้เป็นชื่อที่นายตั้งไว้ใน data_models.h
        int max = selectedCont.maxF; // <--- แก้เป็นชื่อที่นายตั้งไว้ใน data_models.h
        int containerPrice = selectedCont.basePrice;
        std::string type = selectedCont.type; // เรียกค่า type มาเก็บไว้
        std::string size = selectedCont.size;

        if (currentPage == 0){
            ImGui::PushFont(FONT_TITLE);    
            SendYourFeelings();
            ImGui::PopFont();
            
            bool isFull = (selectedFlowers.size() >= (size_t)max);
            float footerHeight = 70.0f; 
            ImGui::BeginChild("FlowerScrollZone", ImVec2(0, -footerHeight), false, ImGuiWindowFlags_None);
            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            ImVec2 windowPos = ImGui::GetCursorScreenPos();

            const int columns = 6;
            const float spacing = 20.0f;     
            const float boxSize = 150.0f;    // เพิ่มขนาดกล่องนิดหน่อยเพื่อให้มีที่วางข้อความ
            const float imgSize = 90.0f;     // ขนาดรูปภาพ

        if (!selection.occasion.empty()) {
        std::stable_sort(flowerList.begin(), flowerList.end(), [&](const Flower& a, const Flower& b) {
            bool aMatches = (a.occasion == selection.occasion);
            bool bMatches = (b.occasion == selection.occasion);
            
            // ถ้า a ตรง แต่ b ไม่ตรง -> a มาก่อน (true)
            if (aMatches && !bMatches) return true;
            // กรณีอื่นๆ ให้คงลำดับเดิมไว้
            return false;
        });
}

        for (int i = 0; i < (int)flowerList.size(); i++) {
            int row = i / columns;
            int col = i % columns;

            ImVec2 boxPos = ImVec2(windowPos.x + (col * (boxSize + spacing)),windowPos.y + (row * (boxSize + spacing)));
            ImVec2 boxMax = ImVec2(boxPos.x + boxSize, boxPos.y + boxSize);

            // ถ้าเต็ม: ใช้สีขาวที่จางมาก (Alpha 60) | ถ้าไม่เต็ม: สีขาวปกติ (Alpha 150)
            ImU32 boxBgCol = isFull ? ImColor(255, 255, 255, 60) : ImColor(255, 255, 255, 150);
            ImU32 borderCol = isFull ? ImColor(200, 200, 200, 100) : ImColor(255, 255, 255, 255);
            // 1. วาดพื้นหลังกล่อง ---
            draw_list->AddRectFilled(boxPos, boxMax,boxBgCol, 12.0f); // กล่องสีขาว
            draw_list->AddRect(boxPos, boxMax,borderCol, 12.0f, 0, 1.0f); // ขอบ

            // 2.วาดรูปดอกไม้ (ขยับตำแหน่งขึ้นโดยเว้นชื่อและราคา)
            if (i < (int)flowerList.size()) {
            ImVec2 imgPos = ImVec2(boxPos.x + (boxSize - imgSize) / 2.0f, boxPos.y + 25.0f);//คำนวณตำแหน่งเริ่มต้น (มุมซ้ายบน)           
            ImVec2 imgMax = ImVec2(imgPos.x + imgSize, imgPos.y + imgSize);// คำนวณตำแหน่งสิ้นสุด (มุมขวาล่าง) 
            draw_list->AddImage((void*)(intptr_t)flowerList[i].texture.getNativeHandle(), imgPos, imgMax);// 4. วาดรูปโดยดึงจาก flowerList 
            }
            // 3. แสดงชื่อและราคาบนสี่เหลี่ยมเลย (ตรงกลาง)
            std::string nameText = flowerList[i].name;//แสดงชื่อดอกไม้
            float nameWidth = ImGui::CalcTextSize(nameText.c_str()).x;
            draw_list->AddText(ImVec2(boxPos.x + (boxSize - nameWidth) / 2.0f, boxPos.y + 10.0f), ImColor(0, 0, 0, 255), nameText.c_str());//ปรับเป็น 110.0f ถ้าต้องการเลื่อนไปด้านล่าง
            std::string priceText = std::to_string(flowerList[i].price) + " THB";// แสดงราคาดอกไม้
            float priceWidth = ImGui::CalcTextSize(priceText.c_str()).x;
            draw_list->AddText(ImVec2(boxPos.x + (boxSize - priceWidth) / 2.0f, boxPos.y + 115.0f),ImColor(0, 0, 0, 255), priceText.c_str());
            std::string occasionText = flowerList[i].occasion;//แสดงชื่อดอกไม้
            std::string formOccasion = "[" + flowerList[i].occasion + "]";
            float occasionWidth = ImGui::CalcTextSize(formOccasion.c_str()).x;
            draw_list->AddText(
            ImVec2(boxPos.x + (boxSize - occasionWidth) / 2.0f, boxPos.y + 132.0f), 
            ImColor(0, 0, 0, 255), 
            formOccasion.c_str()
            );
            
            // 4. ระบบ Interaction & Tooltip สำหรับความหมาย
            ImGui::SetCursorScreenPos(boxPos);
            ImGui::PushID(i);

            if (ImGui::InvisibleButton("FlowerBtn", ImVec2(boxSize, boxSize))) {
            if (selectedFlowers.size() < (size_t)max) {
                selectedFlowers.push_back(flowerList[i]);
            } else {
                openFullPopup = true;
            }
            }

            if (ImGui::IsItemHovered()) {
                // ไฮไลต์สีทองปกติถ้ายังเลือกได้
                draw_list->AddRect(boxPos, boxMax, ImColor(255, 215, 0, 255), 12.0f, 0, 2.0f);
                ImGui::BeginTooltip();
                ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.8f, 1.0f), "Meaning:");
                ImGui::TextWrapped("%s", flowerList[i].meaning.c_str());
                ImGui::EndTooltip();
            }

            ImGui::SetItemAllowOverlap();//ให้คลิกปุ่มบนแถบได้แม้ตำแหน่งทับซ้อนกัน
            ImVec2 winPos = ImGui::GetWindowPos();
            ImVec2 winSize = ImGui::GetWindowSize();
            float barHeight = 60.0f; // ความสูงของแถบสีขาว
            ImVec2 barMin = ImVec2(winPos.x, winPos.y + winSize.y - barHeight);
            bool mouseOnBar = (ImGui::GetIO().MousePos.y >= barMin.y);

            ImGui::PopID();
        }//จบ loop ดอกไม้
        
        ImGui::EndChild();
        //เซตตำแหน่ง pop up
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();// 1.คำนวณจุดกึ่งกลางของหน้าต่างโปรแกรม
        // สั่งให้ Window ถัดไป (ซึ่งคือ Popup) วางตัวตรงกลาง
        // พิกัดคือ center, และใช้ Pivot (0.5f, 0.5f) เพื่อให้จุดกึ่งกลางของ Popup ทับกับจุด center พอดี
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        //วาด popup
        

            // ตั้งค่าเพิ่มเติมการเลื่อนดูดอกไม้
            int totalRows = ((int)flowerList.size() + 5) / 6; 
            float totalHeight = totalRows * (boxSize + spacing);
            ImGui::SetCursorPosY(totalHeight + 20.0f);
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
            totalP=totalP+selectedCont.basePrice;
            for(const auto& f : selectedFlowers) totalP += f.price;

            // --- 2. อ้างอิงพิกัดจากแถบสีขาว (barMin ที่เราคำนวณไว้ก่อนหน้านี้) ---
            // สมมติว่า barMin.y คือจุดเริ่มต้นของแถบขาวด้านล่าง
            float textY = barMin.y + (barHeight - ImGui::GetTextLineHeight()) / 2.0f; 
            float leftPadding = 30.0f; // ระยะห่างจากขอบซ้ายของหน้าต่าง

            // --- 3. วาดจำนวนดอกไม้ (ฝั่งซ้าย) ---
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.85f, 0.45f, 0.55f, 1.0f)); // สีชมพูพาสเทลเข้ม
            std::string txtItems = "Selected: " + std::to_string(selectedFlowers.size())+" / " + std::to_string(selectedCont.maxF) + " pieces";
            ImGui::SetCursorScreenPos(ImVec2(winPos.x + leftPadding, textY)) ;
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
                showBackWarning = true;
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
        OrderSummary();
        ImGui::PopFont();

        // --- 1. เตรียมข้อมูล (เหมือนเดิม) ---
        flowerCounts.clear();
        flowerPrices.clear();
        for (const auto& flower : selectedFlowers) {
            flowerCounts[flower.name]++;
            flowerPrices[flower.name] = flower.price;
        }

        // --- 2. คำนวณขอบเขตของสี่เหลี่ยม (ให้อยู่ตรงกลางกว้าง 2/3) ---
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        ImVec2 winPos = ImGui::GetWindowPos();
        ImVec2 winSize = ImGui::GetWindowSize();

        // คำนวณความกว้าง 2/3 ของหน้าต่าง
        float rectWidth = winSize.x * (2.0f / 3.0f);
        // คำนวณจุดเริ่ม X เพื่อให้อยู่ตรงกลาง (ช่องว่างที่เหลือสองข้างหารสอง)
        float rectX = winPos.x + (winSize.x - rectWidth) * 0.5f;

        // กำหนดตำแหน่ง Y
        float topPadding = 80.0f;    // ระยะจากด้านบน (เผื่อ Title)
        float bottomPadding = 60.0f; // ระยะห่างจากขอบล่าง 20 หน่วยตามโจทย์

        ImVec2 rectMin = ImVec2(rectX, winPos.y + topPadding);
        ImVec2 rectMax = ImVec2(rectX + rectWidth, winPos.y + winSize.y - bottomPadding);

        // วาดสี่เหลี่ยม (พื้นหลังขาวโปร่งแสง)
        drawList->AddRectFilled(rectMin, rectMax, ImColor(255, 255, 255, 180), 15.0f);
        drawList->AddRect(rectMin, rectMax, ImColor(255, 255, 255, 255), 15.0f, 0, 1.5f);

        // --- 3. แสดงข้อมูลภายในสี่เหลี่ยม ---
        // ใช้ CursorPos เพื่อกำหนดให้ ImGui วาดข้อความเริ่มจากมุมซ้ายบนของสี่เหลี่ยม
        float textInsidePadding = 15.0f;
        ImGui::SetCursorScreenPos(ImVec2(rectMin.x + textInsidePadding, rectMin.y + textInsidePadding));

        // เริ่มกลุ่มรายการ (หุ้มด้วย Group เพื่อให้จัดการตำแหน่งง่ายขึ้น)
        ImGui::BeginGroup();
        
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.1f, 0.1f, 0.3f, 1.0f)); // เปลี่ยนสี text เป็นสีเข้มเพื่อให้เด่นบนพื้นขาว
        
        
        ImGui::Text("Your Selection Detail:");

        //วาดเส้นกั้น
        ImVec2 currentCursorPos1 = ImGui::GetCursorScreenPos(); // ดึงตำแหน่งปัจจุบันของตัวหนังสือ
        float lineMargin = 10.0f; // ระยะห่างจากขอบกล่องซ้าย-ขวา
        float lineY1 = currentCursorPos1.y + 5.0f; // ให้เส้นอยู่ต่ำกว่าตัวหนังสือบรรทัดบนเล็กน้อย
        // วาดเส้นจาก (ซ้ายของกล่อง + margin) ไปยัง (ขวาของกล่อง - margin)
        drawList->AddLine(
            ImVec2(rectMin.x + lineMargin, lineY1), 
            ImVec2(rectMax.x - lineMargin, lineY1), 
            ImColor(200, 200, 200, 255), // สีเทา
            1.0f                         // ความหนาของเส้น
        );
        // เลื่อน Cursor ลงมาด้านล่างเส้น เพื่อให้ตัวหนังสือบรรทัดถัดไปไม่ทับเส้น
        ImGui::SetCursorScreenPos(ImVec2(currentCursorPos1.x, lineY1 + 10.0f));

        ImGui::Text("%s (%s)", type.c_str(), size.c_str(), selectedCont.basePrice);

        // ส่วนราคาภาชนะ (ชิดขวาแบบเป๊ะๆ)
        std::string baseprice = std::to_string(selectedCont.basePrice) + " THB";
        float priceTextWidth = ImGui::CalcTextSize(baseprice.c_str()).x;
        // คำนวณ SameLine โดยเอาความกว้างสี่เหลี่ยม ลบความกว้างตัวหนังสือ และลบระยะห่างจากขอบ (Padding)
        float rightPadding = 20.0f; 
        ImGui::SameLine(rectMax.x - rectMin.x - priceTextWidth - rightPadding); 
        ImGui::Text("%s",baseprice.c_str());
        ImVec2 currentCursorPos2 = ImGui::GetCursorScreenPos(); // ดึงตำแหน่งปัจจุบันของตัวหนังสือ
        float lineY2 = currentCursorPos2.y + 5.0f; // ให้เส้นอยู่ต่ำกว่าตัวหนังสือบรรทัดบนเล็กน้อย

        // วาดเส้นจาก (ซ้ายของกล่อง + margin) ไปยัง (ขวาของกล่อง - margin)
        drawList->AddLine(
            ImVec2(rectMin.x + lineMargin, lineY2), 
            ImVec2(rectMax.x - lineMargin, lineY2), 
            ImColor(200, 200, 200, 255), // สีเทา
            1.0f                         // ความหนาของเส้น
        );
        // เลื่อน Cursor ลงมาด้านล่างเส้น เพื่อให้ตัวหนังสือบรรทัดถัดไปไม่ทับเส้น
        ImGui::SetCursorScreenPos(ImVec2(currentCursorPos2.x, lineY2 + 10.0f));

        // แสดงรายการดอกไม้
        int totalFlower = 0;
        for (auto const& [name, count] : flowerCounts) {
            totalFlower +=count;
            int unitPrice = flowerPrices[name];
            int totalForEach = unitPrice * count;
            ImGui::PushID(name.c_str());

            ImGui::Text("%s [%d]", name.c_str(), unitPrice);
            // 3. ส่วนของราคารวม (ชิดขวาแบบเป๊ะๆ)
            std::string priceStr = std::to_string(totalForEach) + " THB";
            float priceTextWidth = ImGui::CalcTextSize(priceStr.c_str()).x;
            
            // คำนวณ SameLine โดยเอาความกว้างสี่เหลี่ยม ลบความกว้างตัวหนังสือ และลบระยะห่างจากขอบ (Padding)
            float rightPadding = 20.0f; 
            ImGui::SameLine(rectMax.x - rectMin.x - priceTextWidth - rightPadding); 
            ImGui::Text("%s", priceStr.c_str());

            ImGui::SameLine(rectMax.x - rectMin.x - 170.0f); //ปรับให้การเพิ่มลดมาอยู่บรรทัดเดียวกับดอกไม้
            // 1. ปุ่มลบ (ลดจำนวน)
            if (ImGui::Button("-", ImVec2(15, 15))) {
                // ค้นหาดอกไม้ชื่อนี้ในตะกร้า แล้วลบออก 1 ชิ้น
                auto it = std::find_if(selectedFlowers.begin(), selectedFlowers.end(), [&](const Flower& f) {
                    return f.name == name;
                });
                if (it != selectedFlowers.end()) {
                    selectedFlowers.erase(it);
                }
            }
            ImGui::SameLine();
            ImGui::Text("%d", count); // แสดงจำนวนปัจจุบัน
            ImGui::SameLine();

            // 2. ปุ่มบวก (เพิ่มจำนวนจากที่มีอยู่แล้ว)
            if (ImGui::Button("+", ImVec2(15, 15))) {
                // เช็คก่อนว่าจำนวนรวมทั้งหมดไม่เกิน Max ของภาชนะ
                if (selectedFlowers.size() < selectedCont.maxF) {
                    
                    // ค้นหาดอกไม้ "ต้นแบบ" จากในตะกร้า selectedFlowers เองเลย
                    auto it = std::find_if(selectedFlowers.begin(), selectedFlowers.end(), [&](const Flower& f) {
                        return f.name == name;
                    });

                    if (it != selectedFlowers.end()) {
                        // ก๊อปปี้ดอกไม้ตัวเดิมที่มีอยู่ในตะกร้า เพิ่มเข้าไปอีก 1
                        selectedFlowers.push_back(*it); 
                    }
                }
                else {
                    openFullPopup = true;
                }
                
            }
            ImGui::PopID();
        }
        void ShowContainerFullPopup(bool& show, int& currentPage, Container& selectedCont, std::vector<Container>& containerList, UserSelection& selection);

        //วาดเส้นกั้น
        ImVec2 totallineCursorPos = ImGui::GetCursorScreenPos(); // ดึงตำแหน่งปัจจุบันของตัวหนังสือ
        float line2Margin = 15.0f; 
        float lineY3 = totallineCursorPos.y + 2.0f;
        drawList->AddLine(
            ImVec2(rectMin.x + line2Margin, lineY3), 
            ImVec2(rectMax.x - line2Margin, lineY3), 
            ImColor(200, 200, 200, 255), // สีเทา
            1.0f                         // ความหนาของเส้น
        );
        selection.selectedFlowers = selectedFlowers;//ส่งข้อมูลไป  flowerrender
        int totalAll = selectedCont.basePrice; // เริ่มต้นด้วยราคาภาชนะ
        for (auto const& [name, count] : flowerCounts) {
            totalAll += (flowerPrices[name] * count); // บวกราคาดอกไม้แต่ละชนิดเข้าไป
        }
        selection.totalAmount = totalAll;
        ImVec2 totalCursorPos = ImGui::GetCursorScreenPos(); // ดึงตำแหน่งปัจจุบันของตัวหนังสือ
        float linetotalY = totallineCursorPos.y + 2.0f;
        // เลื่อน Cursor ลงมาด้านล่างเส้น เพื่อให้ตัวหนังสือบรรทัดถัดไปไม่ทับเส้น
        ImGui::SetCursorScreenPos(ImVec2(totalCursorPos.x, linetotalY + 10.0f));
        ImGui::Text("Total Flower: %d / %d", totalFlower,selectedCont.maxF);
        ImGui::Text("Total Price: %d THB", totalAll);
        ImGui::PopStyleColor(1);
        ImGui::EndGroup();

    
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
            selection.source = EntrySource::CUSTOM; //เก็บข้อมูลว่าเข้ามาจากไหน
            
        int minRequired = 1; // หรือใช้ค่าจากโครงสร้างข้อมูลภาชนะของนาย

        if (selectedFlowers.size() < selectedCont.minF) {
            showWarning = true; // เปิดหน้าต่างเตือนถ้าดอกไม้น้อยเกินไป
        } else {
            selection.source = EntrySource::CUSTOM;
            state = AppState::SUMMARY;
        }
        }
        if (showWarning) {
        ImGui::OpenPopup("Selection Warning");
        }

        //เซตตำแหน่ง pop up
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();// 1.คำนวณจุดกึ่งกลางของหน้าต่างโปรแกรม
        // สั่งให้ Window ถัดไป (ซึ่งคือ Popup) วางตัวตรงกลาง
        // พิกัดคือ center, และใช้ Pivot (0.5f, 0.5f) เพื่อให้จุดกึ่งกลางของ Popup ทับกับจุด center พอดี
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        // วาด Modal Pop-up
        ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.25f, 0.35f, 0.55f, 1.0f)); // สีน้ำเงิน : โฟกัส
        ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.20f, 0.30f, 0.50f, 1.0f)); // สีน้ำเงิน : ไม่โฟกัส
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));// สีขาว : สีข้อความบนแถบ 
        if (ImGui::BeginPopupModal("Selection Warning", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            float buttonWidth = 180.0f; //ขนาดปุ่ม
            float buttonLength1 = 30.0f; //ขนาดปุ่มใหญ่
            float buttonLength2 = 20.0f; //ขนาดปุ่มเล็ก
            ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "Minimum requirement not met!");
            ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "Please select at least %d flowers.", selectedCont.minF);
            ImGui::Separator();
    

            // ทางเลือกที่ 1: กลับไปเลือกดอกไม้เพิ่ม
            if (ImGui::Button("Add More", ImVec2(buttonWidth,buttonLength1))) {
                showWarning = false;
                ImGui::CloseCurrentPopup();
                // ไม่ต้องทำอะไร เพราะเราอยู่ที่หน้าเลือกดอกไม้อยู่แล้ว
            }
            
            ImGui::SameLine();

            //เช็กว่ามีไซส์ภาชนะที่เล็กกว่านี้มั้ย
            bool hasSmaller = false;
            for (auto& c : containerList) {
                if (c.type == selectedCont.type && c.maxF < selectedCont.maxF) {
                    hasSmaller = true;
                    break;
                }
            }
            //ถ้าไม่มีตัวที่เล็กกว่า ให้เริ่มโหมด Disabled (ปุ่มจะซีดและกดไม่ได้) ---
            if (!hasSmaller) {
                ImGui::BeginDisabled(); 
            }

            // ทางเลือกที่ 2: เปลี่ยนไซส์ภาชนะ
            if (ImGui::Button("Change container size", ImVec2(buttonWidth,buttonLength1))) {
                // 1. หาว่าตอนนี้ใช้ Type อะไร (Vase, Bouquet, หรือ Basket)
            std::string currentType = selectedCont.type;
            
            // 2. ลองหา Container ที่มี Type เดียวกัน แต่มีขนาด (Max Flowers) น้อยกว่าตัวปัจจุบัน
            Container* smallerContainer = nullptr;
            int currentMax = selectedCont.maxF;

            for (auto& c : containerList) {
                // ต้องเป็นชนิดเดียวกัน (เช่น Vase เหมือนกัน) 
                // และต้องมีขนาดเล็กลง (maxF น้อยกว่าตัวเดิม)
                if (c.type == currentType && c.maxF < currentMax) {
                    // เราจะเก็บตัวที่ "ใหญ่ที่สุดในบรรดาตัวที่เล็กลง" 
                    // เช่น ถ้ามี S กับ M แล้วปัจจุบันคือ L เราจะเลือก M
                    if (smallerContainer == nullptr || c.maxF > smallerContainer->maxF) {
                        smallerContainer = &c;
                    }
                }
            }

            // 3. ถ้าเจอตัวที่เล็กลง ให้เปลี่ยนทันที
            if (smallerContainer != nullptr) {
                
                selectedCont = *smallerContainer; // อัปเดตตัวแปรหลักที่ใช้ควบคุม Container ในหน้านี้
                // อัปเดตข้อมูลลงใน selection เพื่อให้หน้าอื่นๆ (และตัวแปรตอนเริ่มฟังก์ชัน) เปลี่ยนตาม
                selection.containerType = smallerContainer->type;
                selection.containerSize = smallerContainer->size;
                std::cout << "Downgraded to: " << selectedCont.size << std::endl;
                showWarning = false;
                ImGui::CloseCurrentPopup();
                // ไม่ต้องเปลี่ยน State ก็ได้ เพราะเราเปลี่ยนไซส์ให้เขาเลยในหน้านี้!
            }
        }
        // setting ปุ่ม close
        float posX = (ImGui::GetWindowSize().x - buttonWidth) * 0.5f;// คำนวณหาจุดเริ่มวาดเพื่อให้ปุ่มอยู่กลางหน้าต่าง
        ImGui::SetCursorPosX(posX);// ย้าย Cursor ไปที่ตำแหน่งกลางตามที่คำนวณ
        if (ImGui::Button("Close", ImVec2(buttonWidth,buttonLength2))) {
            ImGui::CloseCurrentPopup();
        }

        if (!hasSmaller) {
            ImGui::EndDisabled();
    }

    ImGui::EndPopup();
        }
        ImGui::PopStyleColor(3);
    }

        // เช็คเพื่อสั่งเปิด Popup
        if (openFullPopup) {
            ImGui::OpenPopup("ContainerFullOptions");
            openFullPopup = false; 
        }
        ShowContainerFullPopup(openFullPopup, currentPage, selectedCont, containerList, selection);
        if (showBackWarning) {
            ImGui::OpenPopup("BackWarningPopup");
            showBackWarning = false; 
        }
        ShowBackWarningPopup(showBackWarning, selectedFlowers, state);
        
        ImGui::PopStyleColor(3); 
        ImGui::PopStyleVar(2); 
        ImGui::End(); 


}
#endif
