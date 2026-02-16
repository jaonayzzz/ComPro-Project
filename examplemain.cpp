#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <iostream>
#include <vector>
#include <optional> // จำเป็นสำหรับ SFML 3 pollEvent

int main() {
    // 1. สร้างหน้าต่าง (SFML 3 ใช้โครงสร้าง VideoMode แบบใหม่)
    sf::RenderWindow window(sf::VideoMode({800, 600}), "My Flower Shop Game - SFML 3.0");
    window.setFramerateLimit(60);

    // 2. เริ่มต้นระบบ ImGui
    if (!ImGui::SFML::Init(window)) return -1;

    // 3. โหลดรูปดอกไม้
    sf::Texture flowerTexture;
    bool hasTexture = false;
    if (flowerTexture.loadFromFile("flower.png")) {
        hasTexture = true;
    }

    // ตัวแปรเก็บข้อมูลเกม
    std::vector<sf::Sprite> flowerSprites;
    std::vector<sf::RectangleShape> flowerShapes;
    int money = 100;
    int seeds = 5;
    sf::Clock deltaClock;

    // 4. ลูปหลักของเกม (Game Loop)
    while (window.isOpen()) {
        // --- ส่วนที่เปลี่ยน: SFML 3 ใช้ pollEvent() คืนค่า std::optional ---
        while (const std::optional event = window.pollEvent()) {
            ImGui::SFML::ProcessEvent(window, *event);

            // เช็คการปิดหน้าต่างด้วย is<T>()
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            // เช็คการคลิกเมาส์ด้วย getIf<T>() เพื่อดึงข้อมูลปุ่มและพิกัด
            if (const auto* mouseBtn = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseBtn->button == sf::Mouse::Button::Left && 
                    !ImGui::GetIO().WantCaptureMouse) { // ป้องกันการปลูกดอกไม้ทะลุเมนู
                    
                    if (seeds > 0) {
                        // ดึงตำแหน่งเมาส์และแปลงเป็น Vector2f
                        sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
                        
                        if (hasTexture) {
                            sf::Sprite newFlower(flowerTexture);
                            newFlower.setOrigin({flowerTexture.getSize().x / 2.0f, flowerTexture.getSize().y / 2.0f});
                            newFlower.setPosition(mousePos);
                            newFlower.setScale({0.2f, 0.2f});
                            flowerSprites.push_back(newFlower);
                        } else {
                            sf::RectangleShape newFlower({40.0f, 40.0f});
                            newFlower.setFillColor(sf::Color::Magenta);
                            newFlower.setOrigin({20.0f, 20.0f});
                            newFlower.setPosition(mousePos);
                            flowerShapes.push_back(newFlower);
                        }
                        seeds--;
                    }
                }
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        // --- สร้างเมนูควบคุม (UI Manager) ---
        ImGui::Begin("Flower Shop Manager");
        ImGui::Text("Money: $%d", money);
        ImGui::Text("Seeds: %d", seeds);
        ImGui::Separator();

        if (ImGui::Button("Buy Seed ($10)")) {
            if (money >= 10) {
                money -= 10;
                seeds++;
            }
        }

        size_t count = hasTexture ? flowerSprites.size() : flowerShapes.size();
        ImGui::Text("Flowers planted: %zu", count);
        
        if (count > 0) {
            if (ImGui::Button("Sell All Flowers (Get $20 each)")) {
                money += static_cast<int>(count) * 20;
                flowerSprites.clear();
                flowerShapes.clear();
            }
        }
        ImGui::End();

        // --- วาดหน้าจอ ---
        window.clear(sf::Color(50, 150, 50)); // สีเขียวหญ้า

        if (hasTexture) {
            for (auto& flower : flowerSprites) window.draw(flower);
        } else {
            for (auto& flower : flowerShapes) window.draw(flower);
        }

        ImGui::SFML::Render(window); 
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}