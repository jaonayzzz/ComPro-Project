#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/System/Clock.hpp>
#include <iostream>
#include <optional>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include "config.h"
#include "font_manager.h"
#include "data_models.h"
#include "app_state.h"
using namespace std;
vector<Flower> buildPool(string targetOccasion){
    vector<Flower> pool;
    for(const auto& f : flowerList){
        if(f.occasion==targetOccasion){
            pool.push_back(f);
        }
    }
    return pool;
}//ฟังก์ชันนี้เราจะคัดดอกไม่ตามโอการมาไว้ใน pool ซึ่งจะเอาไว้ใช้สุ่มดอกออกมาต่อไป

Container selectContainer(const UserSelection& userSel){
    Container result;
    bool foundContainer = false;

    for (const auto& c : containerList){
        if (c.type == userSel.containerType && c.size==userSel.containerSize){
            result = c;
            foundContainer = true;
            break;
        }
    }
    if(foundContainer){
        cout << "Selected : " << result.type << " Size " << result.size << endl;
        cout << "Base Price : " << result.basePrice << " THB" << endl;
        cout << "Flowers   : " << result.minF << " - " << result.maxF << " pieces" << endl;
        cout << "Total Range: " << result.minTotal << " - " << result.maxTotal << " THB" << endl;
    } else { cout << "Error: Container not found!" << endl;
    }

    return result;
}//ฟังก์ชันนี้มีไว้หาพาชนะใส่ดอกม้าย


 
vector<Flower> findBestBouquet(const vector<Flower>& pool, const Container& selectedContainer, int budget, bool prioritizeCount) {
    
    srand(time(0));
    
    int flowerBudget = budget - selectedContainer.basePrice;
    vector<Flower> bestBouquet;
    int bestFlowerCount = 0;
    int minChange = flowerBudget;

    for (int i = 0; i < 1000; ++i) {
        vector<Flower> currentTry;
        int currentTotal = 0;
        int retryCount = 0;

        while ((int)currentTry.size() < selectedContainer.maxF && !pool.empty()) {
            int randomIndex = rand() % pool.size();
            const Flower& picked = pool[randomIndex];

            if (currentTotal + picked.price <= flowerBudget) {
                currentTry.push_back(picked);
                currentTotal += picked.price;
                retryCount = 0;
            } else {
                retryCount++;
                if (retryCount > 30) break;
            }
        }

        if ((int)currentTry.size() >= selectedContainer.minF) {
            int change = flowerBudget - currentTotal;
            int flowerCount = currentTry.size();

            if (prioritizeCount) {
                // สุ่มใหม่: จำนวนดอกสำคัญกว่า
                if (flowerCount > bestFlowerCount ||
                   (flowerCount == bestFlowerCount && change < minChange)) {
                    bestFlowerCount = flowerCount;
                    minChange = change;
                    bestBouquet = currentTry;
                }
            } else {
                // ปกติ: เงินทอนน้อยสำคัญกว่า
                if (change < minChange ||
                   (change == minChange && flowerCount > bestFlowerCount)) {
                    minChange = change;
                    bestFlowerCount = flowerCount;
                    bestBouquet = currentTry;
                }
            }
        }

        if (prioritizeCount && bestFlowerCount == selectedContainer.maxF && minChange == 0) break;
        if (!prioritizeCount && minChange == 0) break;
    }

    return bestBouquet;
}


// แสดงผล
void runBouquetLogic(const UserSelection& userSel, int budget) {
    vector<Flower> pool = buildPool(userSel.occasion);
    Container selectedContainer = selectContainer(userSel);
    vector<Flower> bestBouquet = findBestBouquet(pool, selectedContainer, budget, userSel.isCustomMode);

if (bestBouquet.empty()) {
    cout << "ไม่พบชุดดอกไม้ที่เหมาะสมกับงบประมาณ" << endl;
} else {
    int totalFlowerPrice = 0;
    for (const auto& f : bestBouquet) {
        cout << "- " << f.name /*<< " (" << f.price << " THB)"*/ << endl;
        totalFlowerPrice += f.price;
    }
    //cout << "Flowers Price : " << totalFlowerPrice << " THB" << endl;
    //cout << "Container     : " << selectedContainer.basePrice << " THB" << endl;
    //cout << "GRAND TOTAL   : " << totalFlowerPrice + selectedContainer.basePrice << " THB" << endl;
    //cout << "CHANGE        : " << budget - (totalFlowerPrice + selectedContainer.basePrice) << " THB" << endl;
    cout << "TOTAL FLOWERS : " << bestBouquet.size() << " pieces" << endl;
    }
}

void Random_Pages(sf::RenderWindow& window, AppState& currentState, UserSelection& userSelection){
    static vector<Flower> bestBouquet;
    static Container selectedContainer;
    static bool hasResult = false;
    static int budget = 0;

    
    sf::VertexArray bg(sf::PrimitiveType::TriangleStrip, 4);
    bg[0].position = {0.f, 0.f};
    bg[1].position = {(float)WINDOW_WIDTH, 0.f};
    bg[2].position = {0.f, (float)WINDOW_HEIGHT};
    bg[3].position = {(float)WINDOW_WIDTH, (float)WINDOW_HEIGHT};

    bg[0].color = BG_GRADIENT_TOP_LEFT;
    bg[1].color = BG_GRADIENT_TOP_RIGHT;
    bg[2].color = BG_GRADIENT_BOTTOM_LEFT;
    bg[3].color = BG_GRADIENT_BOTTOM_RIGHT;

    window.draw(bg);
    // DEBUG LINE: เพิ่มเพื่อเช็คว่าค่ามาจริงไหม
    static int actualMinBudget = 0;

    // คำนวณงบขั้นต่ำที่ต้องใช้จริงตามโอกาส
    vector<Flower> pool = buildPool(userSelection.occasion);
    Container currentContainer = selectContainer(userSelection);

    if(!pool.empty()) {
        vector<int> prices;
        for(auto& f : pool) prices.push_back(f.price);
        sort(prices.begin(), prices.end());

        int flowerSum = 0;
        for(int i=0; i < currentContainer.minF && i < prices.size(); i++) flowerSum += prices[i];
        actualMinBudget = currentContainer.basePrice + flowerSum;
    }

    ImGui::Begin("Random Bouquet");
    ImGui::PushFont(FONT_BODY);

    // แสดงข้อมูล Guide ให้ผู้ใช้
    ImGui::Text("Occasion : %s", userSelection.occasion.c_str());
    ImGui::Text("Required Budget for this selection: at least % d THB", actualMinBudget);

    if(budget < actualMinBudget && budget > 0) {
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "Warning: Budget is lower than required min!");
    }

    ImGui::Separator();
    ImGui::InputInt("Budget(THB)", &budget);
    ImGui::Spacing();

    
    if(ImGui::Button("Generate") && budget > 0){
        vector<Flower> pool = buildPool(userSelection.occasion);
        selectedContainer = selectContainer(userSelection);
        bestBouquet = findBestBouquet(pool, selectedContainer, budget, false);
        hasResult = true;
    }

    if(hasResult){
        ImGui::Spacing();
        ImGui::Separator();
        if(bestBouquet.empty()){
            ImGui::TextColored(ImVec4(1, 0, 0, 1), "Best combination couldn't be found");
        } else{
            int totalPrice =0;
            ImGui::Text("=== Bouquet Result ===");
            for(const auto& f :bestBouquet){
                ImGui::BulletText("%s",f.name.c_str());
                totalPrice += f.price;
            }
            ImGui::Separator();
            ImGui::Text("Total Flower : %zu pieces", bestBouquet.size());
            ImGui::Spacing();

            if(ImGui::Button("Re-Random")){
                vector<Flower> pool = buildPool(userSelection.occasion);
                selectedContainer = selectContainer(userSelection);
                bestBouquet = findBestBouquet(pool, selectedContainer, budget, true);
            }
        }
    }
    ImGui::Spacing();
    ImGui::Separator();

    if(ImGui::Button("Back")){
        hasResult = false;
        bestBouquet.clear();
        budget = 0;
        currentState = AppState::MAIN_MENU;
    }
    ImGui::PopFont();
    ImGui::End();
}
