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

inline void SendYourFeelings();
inline void OrderSummary();

inline void Custom_Pages(std::vector<Flower>& flowerList,std::vector<Flower>& selectedFlowers,AppState& state,UserSelection& selection){
    static int currentPage = 0; 
    static float notificationTimer = 0.0f; // 0 คือไม่แสดง, > 0 คือกำลังแสดง
    float displayDuration = 2.0f;          // ให้แสดงค้างไว้ 2 วินาที
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

        for (int i = 0; i < (int)flowerList.size(); i++) {
            int row = i / columns;
            int col = i % columns;

            ImVec2 boxPos = ImVec2(windowPos.x + (col * (boxSize + spacing)), 
                                windowPos.y + (row * (boxSize + spacing)));
            ImVec2 boxMax = ImVec2(boxPos.x + boxSize, boxPos.y + boxSize);

            // ถ้าเต็ม: ใช้สีขาวที่จางมาก (Alpha 60) | ถ้าไม่เต็ม: สีขาวปกติ (Alpha 150)
            ImU32 boxBgCol = isFull ? ImColor(255, 255, 255, 60) : ImColor(255, 255, 255, 150);
            ImU32 borderCol = isFull ? ImColor(200, 200, 200, 100) : ImColor(255, 255, 255, 255);
            // --- 1. วาดพื้นหลังกล่อง ---
            draw_list->AddRectFilled(boxPos, boxMax,boxBgCol, 12.0f); // กล่องสีขาว
            draw_list->AddRect(boxPos, boxMax,borderCol, 12.0f, 0, 1.0f); // ขอบ

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
            if (isFull) {
                ImGui::BeginTooltip();
                ImGui::TextColored(ImVec4(0.8f, 0.2f, 0.2f, 1.0f), "Container Full (%d/%d)", (int)selectedFlowers.size(), max);
                ImGui::EndTooltip();
            } else {
                // ไฮไลต์สีทองปกติถ้ายังเลือกได้
                draw_list->AddRect(boxPos, boxMax, ImColor(255, 215, 0, 255), 12.0f, 0, 2.0f);
                
                ImGui::BeginTooltip();
                ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.8f, 1.0f), "Meaning:");
                ImGui::TextWrapped("%s", flowerList[i].meaning.c_str());
                ImGui::EndTooltip();
            }
    }
            ImGui::SetItemAllowOverlap();//ให้คลิกปุ่มบนแถบได้แม้ตำแหน่งทับซ้อนกัน
            ImVec2 winPos = ImGui::GetWindowPos();
            ImVec2 winSize = ImGui::GetWindowSize();
            float barHeight = 60.0f; // ความสูงของแถบสีขาว
            ImVec2 barMin = ImVec2(winPos.x, winPos.y + winSize.y - barHeight);
            bool mouseOnBar = (ImGui::GetIO().MousePos.y >= barMin.y);
            if (ImGui::IsItemClicked() && ImGui::GetIO().MousePos.y < barMin.y) {
                if (!isFull) {
            selectedFlowers.push_back(flowerList[i]);
            } else {
                // ถ้าเต็มแล้ว ให้เริ่มจับเวลาเพื่อแสดง Pop-up
                notificationTimer = (float)ImGui::GetTime();
            }
                
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
        OrderSummary();
        ImGui::PopFont();

        // --- 1. เตรียมข้อมูล (เหมือนเดิม) ---
        flowerCounts.clear();
        flowerPrices.clear();
        for (const auto& flower : selectedFlowers) {
            flowerCounts[flower.name]++;
            flowerPrices[flower.name] = flower.price;
        }

        // --- 2. คำนวณขอบเขตของสี่เหลี่ยม (ฝั่งซ้าย) ---
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        ImVec2 winPos = ImGui::GetWindowPos();
        ImVec2 winSize = ImGui::GetWindowSize();

        float padding = 20.0f; 
        float centerX = winPos.x + (winSize.x / 2.0f); // เส้นกึ่งกลางหน้าต่าง
        
        // พิกัดสี่เหลี่ยม: เริ่มจากขอบซ้ายหน้าต่าง จนถึง (กึ่งกลาง - 10.0f)
        ImVec2 rectMin = ImVec2(winPos.x + padding, winPos.y + 100.0f); // 100.0f คือเผื่อที่ให้ Title "Summary"
        ImVec2 rectMax = ImVec2(centerX - 10.0f, winPos.y + winSize.y - 80.0f); // 80.0f คือเผื่อที่ให้ปุ่ม Back/Next

        // วาดสี่เหลี่ยมขอบมน (พื้นหลังขาวโปร่งแสงเล็กน้อยเพื่อให้ตัวหนังสือดำอ่านง่าย)
        drawList->AddRectFilled(rectMin, rectMax, ImColor(255, 255, 255, 180), 15.0f);
        drawList->AddRect(rectMin, rectMax, ImColor(255, 255, 255, 255), 15.0f, 0, 1.5f); // เส้นขอบ

        // --- 3. แสดงข้อมูลภายในสี่เหลี่ยม ---
        // ใช้ CursorPos เพื่อกำหนดให้ ImGui วาดข้อความเริ่มจากมุมซ้ายบนของสี่เหลี่ยม
        float textInsidePadding = 15.0f;
        ImGui::SetCursorScreenPos(ImVec2(rectMin.x + textInsidePadding, rectMin.y + textInsidePadding));

        // เริ่มกลุ่มรายการ (หุ้มด้วย Group เพื่อให้จัดการตำแหน่งง่ายขึ้น)
        ImGui::BeginGroup();
        
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.1f, 0.1f, 0.3f, 1.0f)); // เปลี่ยนสี text เป็นสีเข้มเพื่อให้เด่นบนพื้นขาว
        
        
        ImGui::Text("Your Selection Detail:");
        ImGui::BulletText("%s (%s) - %d THB", type.c_str(), size.c_str(), selectedCont.basePrice);
        ImVec2 currentCursorPos = ImGui::GetCursorScreenPos(); // ดึงตำแหน่งปัจจุบันของตัวหนังสือ

        //วาดเส้นกั้น
        float lineMargin = 10.0f; // ระยะห่างจากขอบกล่องซ้าย-ขวา
        float lineY = currentCursorPos.y + 5.0f; // ให้เส้นอยู่ต่ำกว่าตัวหนังสือบรรทัดบนเล็กน้อย
        // วาดเส้นจาก (ซ้ายของกล่อง + margin) ไปยัง (ขวาของกล่อง - margin)
        drawList->AddLine(
            ImVec2(rectMin.x + lineMargin, lineY), 
            ImVec2(rectMax.x - lineMargin, lineY), 
            ImColor(200, 200, 200, 255), // สีเทา
            1.0f                         // ความหนาของเส้น
        );
        // เลื่อน Cursor ลงมาด้านล่างเส้น เพื่อให้ตัวหนังสือบรรทัดถัดไปไม่ทับเส้น
        ImGui::SetCursorScreenPos(ImVec2(currentCursorPos.x, lineY + 10.0f));

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
            }
            ImGui::PopID();
        }
        //วาดเส้นกั้น
        ImVec2 totallineCursorPos = ImGui::GetCursorScreenPos(); // ดึงตำแหน่งปัจจุบันของตัวหนังสือ
        float line2Margin = 15.0f; 
        float line2Y = totallineCursorPos.y + 2.0f;
        drawList->AddLine(
            ImVec2(rectMin.x + line2Margin, line2Y), 
            ImVec2(rectMax.x - line2Margin, line2Y), 
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
        ImGui::Text("Total Flower: %d", totalFlower);
        ImGui::Text("Total Price: %d", totalAll);
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
            state = AppState::SUMMARY;
        }
    }


        ImGui::PopStyleColor(3); 
        ImGui::PopStyleVar(2); 
        ImGui::End(); 


}
#endif
