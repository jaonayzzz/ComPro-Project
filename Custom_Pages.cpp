#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/System/Clock.hpp>
#include <iostream>
#include <optional>

#include "config.h"

void Selectyourpieces();

void Custom_Pages(){
       
        int currentPage=0;
        //setting แถบ flower shop
        ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.05f, 0.10f, 0.20f, 1.0f)); // สีแถบตอนที่กำลังใช้งาน
        ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.10f, 0.15f, 0.30f, 1.0f)); // สีแถบตอนที่ไม่ได้คลิก 
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 15.0f);// ปรับความโค้งมนของมุมหน้าต่าง
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);// ปรับความหนาของเส้นขอบให้บางลงหรือหายไป
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.96f, 0.96f, 0.96f, 1.0f)); //ปรับสีพื้นหลังหน้าต่าง : silver
        ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Always);// กำหนดตำแหน่ง (X, Y) 
        ImGui::SetNextWindowSize(ImVec2(WINDOW_WIDTH-40,WINDOW_HEIGHT-40), ImGuiCond_Always);// fixed ขนาดโดยใช้ ImGuiCond_Always
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); //สี text "Flower shop" : ขาว
        ImGui::Begin("Flower Shop", nullptr, ImGuiWindowFlags_NoResize);// ใส่'ImGuiWindowFlags_NoResize' ในฟังก์ชัน Begin , NULL คือพารามิเตอร์สำหรับปุ่มปิด (ถ้าไม่ใช้ให้ใส่ NULL หรือ nullptr)
        ImGui::PopStyleColor(1); //ล้างสี Text"flower shop" 
        
        if (currentPage == 0){
            Selectyourpieces();

        //การวาด Grid สี่เหลี่ยมขอบมน 
        int rows = 6;            
        int columns = 6;         
        float boxSpacingX = 20.0f;
        float boxSpacingY = 40.0f; 
        float boxSize = ((WINDOW_WIDTH*0.9)-(columns-1)*boxSpacingX)/columns;

        // คำนวณ StartX ให้กึ่งกลาง (อิงจาก Cursor เพื่อให้ Scroll ได้)
        float totalContentWidth = (columns * boxSize) + ((columns - 1) * boxSpacingX);
        ImVec2 currentCursorPos = ImGui::GetCursorScreenPos();
        float startX = currentCursorPos.x + (ImGui::GetContentRegionAvail().x - totalContentWidth) * 0.5f;
        float startY = currentCursorPos.y + 20.0f;

        ImDrawList* draw_list = ImGui::GetWindowDrawList();

        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < columns; c++) {
                int i = r * columns + c;
                ImGui::PushID(i);

                float currentX = startX + (c * (boxSize + boxSpacingX));
                float currentY = startY + (r * (boxSize + boxSpacingY));

                // วาดสี่เหลี่ยม Silver
                draw_list->AddRectFilled(
                    ImVec2(currentX, currentY), 
                    ImVec2(currentX + boxSize, currentY + boxSize), 
                    ImColor(0.88f, 0.89f, 0.91f, 1.0f), 20.0f
                );

                // ปุ่ม Add ภายในสี่เหลี่ยม
                float btnW = 0.3*boxSize;
                float btnH = 0.1*boxSize;
                ImGui::SetCursorScreenPos(ImVec2(currentX + (boxSize * 0.5f) - (btnW * 0.5f), currentY + boxSize - (btnH + 10.0f)));
                    
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.45f, 0.45f, 1.0f)); // สีปุ่มปกติ
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.6f, 0.6f, 1.0f)); //สีเมื่อวางเมาส์: เขียว
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.35f, 0.35f, 1.0f)); // สีตอนกดปุ่ม: เขียว
                ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 15.0f); // ปรับความโค้งของปุ่มให้ดูละมุน (Rounding)
                if (ImGui::Button("Add", ImVec2(btnW, btnH))) {
                    std::cout << "Added flower #" << i + 1 << " to cart!" << std::endl;
                    //std::cin >> ; 
                }
                ImGui::PopStyleVar(1);
                ImGui::PopStyleColor(3);
                ImGui::PopID();
                }
            }
            // setting ปุ่ม "Next"
            // ย้าย Cursor ไปต่อท้ายเนื้อหาเพื่อให้ปุ่ม Next เลื่อนตาม
            ImGui::SetCursorScreenPos(ImVec2(startX, startY + (rows * (boxSize + boxSpacingY)) + 20.0f));

            // ปุ่ม Next (ไหลตามเนื้อหา)
            float nextBtnW = 100.0f;
            ImGui::SetCursorPosX(ImGui::GetWindowSize().x - nextBtnW - 30.0f);
            
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.45f, 0.45f, 1.0f)); // สีปุ่มปกติ
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.6f, 0.6f, 1.0f)); //สีเมื่อวางเมาส์: เขียว
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.35f, 0.35f, 1.0f)); // สีตอนกดปุ่ม: เขียว
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f); // ปรับความโค้งของปุ่มให้ดูละมุน (Rounding)
            if (ImGui::Button("Next >>", ImVec2(nextBtnW, 40))) {
                currentPage = 1;
            }
            ImGui::PopStyleVar(1); //ล้างสี Next
            ImGui::PopStyleColor(3); //ล้างสี Next
        }

        else if (currentPage == 1){
        // setting "Summary of your order"
        float textWidth = ImGui::CalcTextSize("Summary of your order").x; // คำนวณหาความกว้างของข้อความ
        float centerX = (ImGui::GetWindowSize().x - textWidth*1.5) * 0.5f; // หาตำแหน่งที่เริ่มพิมพ์ 
        ImGui::SetCursorPosX(centerX); // ย้าย Cursor ไปที่ตำแหน่งนั้นก่อนพิมพ์ Text
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.1f, 0.1f, 0.44f, 1.0f)); //สี text
        ImGui::SetWindowFontScale(1.5f); // ขยายตัวอักษรทั้งหน้าต่างเป็น 1.5 เท่า
        ImGui::Text("Summary of your order", ImVec2(100, 80));
        ImGui::SetWindowFontScale(1.0f); // คืนค่ากลับเป็นปกติ
        ImGui::PopStyleColor(1); //ล้างสี

        // Setting "Back"
        float windowW = ImGui::GetWindowSize().x;
        float windowH = ImGui::GetWindowSize().y;
        float btnW = 100.0f;
        float btnH = 40.0f;
        float paddingX = 20.0f; // ระยะห่างจากขอบ (ปรับให้เท่ากับหน้าแรก)
        float paddingY = 10.0f; // ระยะห่างจากขอบ (ปรับให้เท่ากับหน้าแรก)
        ImGui::SetCursorPos(ImVec2(windowW - btnW - paddingX, windowH - btnH - paddingY));// ตั้งค่าตำแหน่ง (อ้างอิงภายใน Window)
        // วาดปุ่ม Back พร้อมสไตล์
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.20f, 0.30f, 0.60f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.30f, 0.45f, 0.80f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.15f, 0.20f, 0.45f, 1.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f);
        if (ImGui::Button("<< Back", ImVec2(btnW, btnH))) {
            currentPage = 0; // กลับไปหน้าเลือกดอกไม้
        }
        ImGui::PopStyleVar(1); // ล้างสีปุ่ม 
        ImGui::PopStyleColor(3);
                }

        //ล้างสี ปุ่ม"flower shop"
        ImGui::PopStyleColor(3); // ล้างสีพื้นหลัง flower shop 
        ImGui::PopStyleVar(2); // ล้างสีพื้นหลัง flower shop 
        ImGui::End(); // จบการทำงาน แถบ flower shop 
    
}
void Selectyourpieces(){
    float textWidth = ImGui::CalcTextSize("Select your pieces").x; // คำนวณหาความกว้างของข้อความ
        float centerX = (ImGui::GetWindowSize().x - textWidth*1.5) * 0.5f; // หาตำแหน่งที่เริ่มพิมพ์ 
        ImGui::SetCursorPosX(centerX); // ย้าย Cursor ไปที่ตำแหน่งนั้นก่อนพิมพ์ Text
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.1f, 0.1f, 0.44f, 1.0f)); //สี text
        ImGui::SetWindowFontScale(1.5f); // ขยายตัวอักษรทั้งหน้าต่างเป็น 1.5 เท่า
        ImGui::Text("Select your pieces", ImVec2(100, 80));
        ImGui::SetWindowFontScale(1.0f); // คืนค่ากลับเป็นปกติ
        ImGui::PopStyleColor(1); //ล้างสี text
}
