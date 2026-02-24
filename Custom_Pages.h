#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/System/Clock.hpp>
#include <iostream>
#include <optional>
#include <vector>
#include <string>

#include "config.h"

void Selectyourpieces();

void Custom_Pages(std::vector<sf::Texture>& flowerTextures){
    static int currentPage = 0; 
    
    static std::vector<std::string> flowerNames = {
        "Lotus", "White-Carnation", "White-Chrysanthemum", "White-Rose", "White-Orchid", "White-Tulip",
        "Red-Poppy", "White-Lily", "White-Hydrangea", "White-Anemone", "Marigold", "White-Daisy",
        "Yellow-Daisy", "Sunflower", "Pink-Carnation", "Yellow-Chrysanthemum", "Yellow-Rose", "Purple-Dahlia",
        "Purple-Orchid", "Yellow-Tulip", "Purple-Iris", "Red-Peony", "Jasmine", "Red-Carnation",
        "Red-Rose", "Pink-Rose", "Red-Dahlia", "Lavender", "Chamomile", "Red-Tulip",
        "Forget-Me-Not", "Pink-Lily", "Blue-Hydrangea", "Pink-Hydrangea", "Red-Anemone", "Pink-Peony"
    };
   
       
        ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.05f, 0.10f, 0.20f, 1.0f)); 
        ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.10f, 0.15f, 0.30f, 1.0f)); 
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 15.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.96f, 0.96f, 0.96f, 1.0f)); 
        ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Always); 
        ImGui::SetNextWindowSize(ImVec2(WINDOW_WIDTH-40, WINDOW_HEIGHT-40), ImGuiCond_Always);
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); 
        ImGui::Begin("Flower Shop", nullptr, ImGuiWindowFlags_NoResize);
        ImGui::PopStyleColor(1); 
        
        if (currentPage == 0){
            Selectyourpieces();

            int rows = 6;            
            int columns = 6;         
            float boxSpacingX = 20.0f;
            float boxSpacingY = 40.0f; 
            float boxSize = ((WINDOW_WIDTH*0.9f)-(columns-1)*boxSpacingX)/columns;

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

                    draw_list->AddRectFilled(
                        ImVec2(currentX, currentY), 
                        ImVec2(currentX + boxSize, currentY + boxSize), 
                        ImColor(0.88f, 0.89f, 0.91f, 1.0f), 20.0f
                    );

                    float imgSize = boxSize * 0.65f; 
                    float imgPosX = currentX + (boxSize - imgSize) * 0.5f; 
                    float imgPosY = currentY + (boxSize*0.7f - imgSize) * 0.4f; 

                    // --- เช็ก i ก่อนดึงรูป ---
                    if (i < (int)flowerTextures.size()) {
                        ImGui::SetCursorScreenPos(ImVec2(imgPosX, imgPosY));
                        ImGui::Image(flowerTextures[i], sf::Vector2f(imgSize, imgSize));
                    }

                    float btnW = 0.3f*boxSize;
                    float btnH = 0.2f*boxSize;
                    ImGui::SetCursorScreenPos(ImVec2(currentX + (boxSize * 0.5f) - (btnW * 0.5f), currentY + boxSize - (btnH + 10.0f)));
                    
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.45f, 0.45f, 1.0f)); 
                    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 15.0f); 
                    if (ImGui::Button("Add", ImVec2(btnW, btnH))) {
                        std::cout << "Added: " << flowerNames[i] << " to cart!" << std::endl;
                    }
                    ImGui::PopStyleVar(1);
                    ImGui::PopStyleColor(1);
                    ImGui::PopID();
                }
            }

            ImGui::SetCursorScreenPos(ImVec2(startX, startY + (rows * (boxSize + boxSpacingY)) + 20.0f));
            float nextBtnW = 100.0f;
            ImGui::SetCursorPosX(ImGui::GetWindowSize().x - nextBtnW - 30.0f);
            
            if (ImGui::Button("Next >>", ImVec2(nextBtnW, 40))) {
                currentPage = 1;
            }
        }
        else if (currentPage == 1) {
            // โค้ดหน้า Summary (คงเดิม)
            if (ImGui::Button("<< Back", ImVec2(100, 40))) 
            currentPage = 0;
        }

        ImGui::PopStyleColor(3); 
        ImGui::PopStyleVar(2); 
        ImGui::End(); 

    }

void Selectyourpieces() {
    float textWidth = ImGui::CalcTextSize("Select your pieces").x;
    float centerX = (WINDOW_WIDTH*0.9f - textWidth*1.5f) * 0.5f; 
    ImGui::SetCursorPosX(centerX); 
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.1f, 0.1f, 0.44f, 1.0f)); 
    ImGui::SetWindowFontScale(1.5f); 
    ImGui::Text("Select your pieces");
    ImGui::SetWindowFontScale(1.0f); 
    ImGui::PopStyleColor(1); 
}