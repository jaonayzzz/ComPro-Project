#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/System/Clock.hpp>
#include <iostream>
#include <optional>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include "Config.h"
#include "core/FontManager.h"
#include "DataModels.h"
#include "AppState.h"
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


 
// เพิ่มส่วนนี้ในไฟล์ที่ใช้สุ่ม (เช่น Random_Pages.cpp หรือไฟล์ logic)
vector<Flower> findBestBouquet(const vector<Flower>& pool, const Container& selectedContainer, int budget, bool prioritizeCount) {
    srand(static_cast<unsigned>(time(0)));
    int flowerBudget = budget - selectedContainer.basePrice;
    
    
    // กรองดอกไม้ที่งบพอซื้อได้
    vector<Flower> affordablePool;
    for(const auto& f : pool) {
        if(f.price <= flowerBudget) affordablePool.push_back(f);
    }

    if (affordablePool.empty()) return {};

    // เรียงลำดับจากถูกไปแพง เผื่อไว้ใช้ตอนงบจำกัดมาก
    sort(affordablePool.begin(), affordablePool.end(), [](const Flower& a, const Flower& b) {
        return a.price < b.price;
    });
    int acceptableChange = affordablePool[0].price;
    vector<Flower> bestBouquet;
    int minChange = flowerBudget;

    for (int i = 0; i < 2000; ++i) {
        vector<Flower> currentTry;
        int currentTotal = 0;

        // พยายามเติมให้ถึงจำนวนขั้นต่ำ (minF)
        while ((int)currentTry.size() < selectedContainer.minF) {

            int remainingFlowers = selectedContainer.minF - currentTry.size();
            int remainingBudget = flowerBudget - currentTotal;

            const Flower* picked = nullptr;

            if (remainingBudget / affordablePool[0].price == remainingFlowers) {
             picked = &affordablePool[0];
            } else {
                int randomIndex = rand() % affordablePool.size();
                picked = &affordablePool[randomIndex];
            }

            if (currentTotal + picked->price <= flowerBudget) {
                currentTry.push_back(*picked);
                currentTotal += picked->price;
            } else { break;
            }
        }



        // ตรวจสอบว่าผ่านเงื่อนไขขั้นต่ำไหม
        if ((int)currentTry.size() >= selectedContainer.minF) {
            // ลองใส่เพิ่มจนถึง maxF
            while ((int)currentTry.size() < selectedContainer.maxF) {
                int randomIndex = rand() % affordablePool.size();
                if (currentTotal + affordablePool[randomIndex].price <= flowerBudget) {
                    currentTotal += affordablePool[randomIndex].price;
                    currentTry.push_back(affordablePool[randomIndex]);
                } else break;
            }

            int change = flowerBudget - currentTotal;
            if (change < minChange) {
                minChange = change;
                bestBouquet = currentTry;
            }
            if (minChange <= acceptableChange) break;
        }
    }
    return bestBouquet;
}


// แสดงผล
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
    // ภายในฟังก์ชัน Random_Pages
static int actualMinBudget = 0;
static int actualMaxBudget = 0;

// ดึงค่า Container ที่เลือกอยู่ปัจจุบัน
Container currentContainer = selectContainer(userSelection);

// เรียกใช้ฟังก์ชันคำนวณจาก data_models
BudgetRange range = calculateActualBudget(userSelection.occasion, currentContainer);
actualMinBudget = range.min;
actualMaxBudget = range.max;

ImGui::Begin("Random Bouquet");
ImGui::PushFont(FONT_BODY);

// แสดงข้อมูล Guide ให้ผู้ใช้เห็นช่วงราคาที่ "จัดได้จริง"
ImGui::Text("Occasion : %s", userSelection.occasion.c_str());
ImGui::Text("Container: %s (%s)", currentContainer.type.c_str(), currentContainer.size.c_str());

ImGui::Separator();
ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.0f, 1.0f), "Price Guide for this selection:");
ImGui::BulletText("Minimum to start: %d THB", actualMinBudget);
ImGui::BulletText("Recommended max : %d THB", actualMaxBudget);

// ตรวจสอบเงื่อนไขงบประมาณ
if(budget > 0) {
    if(budget < actualMinBudget) {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), 
            "X Budget too low! Need at least %d THB", actualMinBudget);
    } else if (budget > actualMaxBudget * 1.5) {
        ImGui::TextColored(ImVec4(0.0f, 0.8f, 1.0f, 1.0f), 
            "i High budget! You might get many expensive flowers.");
    }
}

    ImGui::Separator();
    ImGui::InputInt("Budget(THB)", &budget);
    ImGui::Spacing();

    
    if(ImGui::Button("Generate") && budget > 0){
        vector<Flower> pool = buildPool(userSelection.occasion);
        selectedContainer = selectContainer(userSelection);
        bestBouquet = findBestBouquet(pool, selectedContainer, budget, false);
    
        // --- จุดที่เพิ่ม: บันทึกค่าลงข้อมูลกลาง ---
        userSelection.selectedFlowers = bestBouquet; 
        userSelection.totalAmount = budget; // หรือคำนวณจากราคาดอกไม้จริง + basePrice
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
                userSelection.selectedFlowers = bestBouquet;
            }
        }
    }
    ImGui::Spacing();
    ImGui::Separator();

    // ภายใน Random_Pages.cpp ส่วนที่แสดงผลลัพธ์
if (hasResult && !bestBouquet.empty()) {
    if (ImGui::Button("Confirm Order")) {
        // บันทึกดอกไม้ลงใน UserSelection ก่อนย้ายหน้า
        userSelection.selectedFlowers = bestBouquet;
        bestBouquet.clear();
        budget = 0;
        currentState = AppState::SUMMARY;
    }
}

    if(ImGui::Button("Back")){
        hasResult = false;
        bestBouquet.clear();
        budget = 0;
        currentState = AppState::MAIN_MENU;
    }
    ImGui::PopFont();
    ImGui::End();
}