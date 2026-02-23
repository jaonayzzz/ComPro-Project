#include "Custom_Page.h"
#include "FlowerRenderer.h"
#include "data_models.h"
#include "config.h"
#include "font_manager.h"

#include <imgui.h>
#include <imgui-SFML.h>
#include <vector>
#include <iostream>
#include <algorithm>

// ======================================================
// ตะกร้าสินค้าและสถานะหน้า (Internal State)
// ======================================================
static std::vector<Flower> cart;
static int currentPage = 0; // 0 = หน้าร้านค้า, 1 = หน้าสรุปรายการ

// ======================================================
// ฟังก์ชันเรียงลำดับดอกไม้ (Sort by Occasion Priority)
// ======================================================
static std::vector<Flower> sortByOccasionPriority(const std::string& selectedOccasion) {
    std::vector<Flower> primary;
    std::vector<Flower> secondary;

    for (const auto& f : flowerList) {
        if (f.occasion == selectedOccasion)
            primary.push_back(f);
        else
            secondary.push_back(f);
    }
    primary.insert(primary.end(), secondary.begin(), secondary.end());
    return primary;
}

// ======================================================
// MAIN SHOP UI
// ======================================================
void drawShop(sf::RenderWindow& window, AppState& state, UserSelection& selection) {
    // ล็อคขนาดหน้าจอเสมือน (Virtual Resolution) เพื่อไม่ให้อัตราส่วนเพี้ยน
    const float V_WIDTH = 1280.f;
    const float V_HEIGHT = 720.f;

    // --- 1. วาดพื้นหลัง Gradient ---
    sf::VertexArray bg(sf::PrimitiveType::TriangleStrip, 4);
    bg[0].position = {0, 0}; bg[1].position = {V_WIDTH, 0};
    bg[2].position = {0, V_HEIGHT}; bg[3].position = {V_WIDTH, V_HEIGHT};
    bg[0].color = BG_GRADIENT_TOP_LEFT; bg[1].color = BG_GRADIENT_TOP_RIGHT;
    bg[2].color = BG_GRADIENT_BOTTOM_LEFT; bg[3].color = BG_GRADIENT_BOTTOM_RIGHT;
    window.draw(bg);

    // --- 2. ตั้งค่า ImGui Overlay (ไร้หน้าต่าง) ---
    ImGui::SetNextWindowPos({0, 0});
    ImGui::SetNextWindowSize({V_WIDTH, V_HEIGHT});
    ImGui::Begin("ShopOverlay", nullptr, 
        ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove);

    ImGui::PushFont(FONT_BODY);

    if (currentPage == 0) {
        // ==================================================
        // PAGE 0 — SELECT FLOWERS
        // ==================================================
        
        // Header
        ImGui::SetCursorPos({50, 40});
        ImGui::PushFont(FONT_TITLE);
        ImGui::TextColored(ImColor(255, 255, 255), "DESIGN YOUR BOUQUET");
        ImGui::PopFont();

        ImGui::SetCursorPos({50, 85});
        ImGui::Text("Occasion Mode: %s", selection.occasion.empty() ? "General" : selection.occasion.c_str());

        // ปุ่มย้อนกลับ
        ImGui::SetCursorPos({V_WIDTH - 180, 40});
        if (ImGui::Button("<< Back", {130, 40})) {
            state = AppState::MAIN_MENU;
        }

        // --- พื้นที่เลื่อนดูดอกไม้ (Scroll Area) ---
        ImGui::SetCursorPos({0, 140});
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0, 0, 0, 0.2f)); 
        // แก้ไข Flag เป็น AlwaysVerticalScrollbar ตามที่ระบบแจ้ง Error
        ImGui::BeginChild("FlowerScrollArea", {V_WIDTH, 460}, false, ImGuiWindowFlags_AlwaysVerticalScrollbar);

        std::vector<Flower> flowers = sortByOccasionPriority(selection.occasion);
        int cols = 5;
        float boxSize = 190.f;
        float gapX = 30.f;
        float gapY = 40.f;
        float startX = (V_WIDTH - (cols * boxSize + (cols - 1) * gapX)) / 2.f;

        for (int i = 0; i < (int)flowers.size(); ++i) {
            int r = i / cols;
            int c = i % cols;
            float posX = startX + c * (boxSize + gapX);
            float posY = r * (boxSize + gapY);

            ImGui::PushID(i);
            ImGui::SetCursorPos({posX, posY});
            
            // --- ลำดับเลเยอร์ 1: วาด Card Background ก่อน ---
            ImDrawList* dl = ImGui::GetWindowDrawList();
            ImVec2 p1 = ImGui::GetCursorScreenPos();
            ImVec2 p2 = {p1.x + boxSize, p1.y + boxSize};
            
            bool isPrimary = (flowers[i].occasion == selection.occasion);
            // ปรับสีการ์ดให้มืดลง (Dark Theme) เพื่อให้ดอกไม้เด่นขึ้น
            ImColor cardColor = isPrimary ? ImColor(180, 60, 100, 200) : ImColor(40, 40, 45, 180);
            
            dl->AddRectFilled(p1, p2, cardColor, 15.f);
            if(isPrimary) dl->AddRect(p1, p2, ImColor(255, 255, 255, 180), 15.f, 0, 2.5f);

            // --- ลำดับเลเยอร์ 2: วาดดอกไม้ทับบน Card ---
            // เรียกใช้ FlowerRenderer หลังจากวาด RectFilled เพื่อให้ดอกไม้อยู่เลเยอร์บน
            FlowerRenderer::draw(window, flowers[i].type, 
                sf::Vector2f(p1.x + boxSize/2.f, p1.y + boxSize/2.f - 15.f), 90.f, 0.f);

            // --- ลำดับเลเยอร์ 3: วาดตัวหนังสือและปุ่ม ---
            ImGui::SetCursorPos({posX + 15, posY + boxSize - 55});
            ImGui::Text("%s", flowers[i].name.c_str());
            
            ImGui::SetCursorPos({posX + 15, posY + boxSize - 35});
            ImGui::TextColored(ImColor(255, 215, 0), "%d THB", flowers[i].price);

            ImGui::SetCursorPos({posX + boxSize - 55, posY + boxSize - 40});
            if (ImGui::Button("+", {45, 30})) {
                cart.push_back(flowers[i]);
            }
            ImGui::PopID();
        }
        ImGui::EndChild();
        ImGui::PopStyleColor();

        // Footer
        ImGui::SetCursorPos({50, V_HEIGHT - 70});
        ImGui::Text("Bouquet Items: %d", (int)cart.size());

        ImGui::SetCursorPos({V_WIDTH - 220, V_HEIGHT - 85});
        if (ImGui::Button("Checkout >", {170, 55})) {
            if(!cart.empty()) currentPage = 1;
        }
    } 
    else {
        // ==================================================
        // PAGE 1 — ORDER SUMMARY (แบบใบเสร็จ)
        // ==================================================
        ImGui::SetNextWindowPos({V_WIDTH/2.f, V_HEIGHT/2.f}, ImGuiCond_Always, {0.5f, 0.5f});
        ImGui::SetNextWindowSize({500, 600});
        
        ImGui::Begin("Receipt", nullptr, ImGuiWindowFlags_NoDecoration);
        
        ImGui::PushFont(FONT_TITLE);
        ImGui::Text("Order Summary");
        ImGui::PopFont();
        ImGui::Separator();
        
        int total = 0;
        ImGui::BeginChild("ItemsList", {0, 380}, true);
        for (int i = 0; i < (int)cart.size(); ++i) {
            ImGui::Text("%d. %s", i + 1, cart[i].name.c_str());
            ImGui::SameLine(ImGui::GetWindowWidth() - 90);
            ImGui::Text("%d", cart[i].price);
            total += cart[i].price;
        }
        ImGui::EndChild();

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Text("Total Price:");
        ImGui::SameLine(ImGui::GetWindowWidth() - 120);
        ImGui::TextColored(ImColor(0, 200, 0), "%d THB", total);

        ImGui::Spacing();
        if (ImGui::Button("<< Edit", {120, 45})) currentPage = 0;
        ImGui::SameLine();
        if (ImGui::Button("Confirm Order", {330, 45})) {
            cart.clear();
            currentPage = 0;
            state = AppState::MAIN_MENU;
        }
        ImGui::End();
    }

    ImGui::PopFont();
    ImGui::End(); 
}