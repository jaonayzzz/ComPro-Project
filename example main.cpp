#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include "imgui.h"
#include "imgui-SFML.h" 

#include <iostream>
#include <vector>

int main() {
    // 1. สร้างหน้าต่างเกม
    sf::RenderWindow window(sf::VideoMode(800, 600), "My Flower Shop Game");
    window.setFramerateLimit(60);

    // 2. เริ่มต้นระบบ ImGui
    ImGui::SFML::Init(window);

    // 3. โหลดรูปดอกไม้ (ถ้าไม่มีรูป จะใช้สี่เหลี่ยมสีชมพูแทน)
    sf::Texture flowerTexture;
    bool hasTexture = false;
    if (flowerTexture.loadFromFile("flower.png")) {
        hasTexture = true;
    }

    // ตัวแปรเก็บข้อมูลเกม
    std::vector<sf::Sprite> flowerSprites;      // เก็บรูปดอกไม้
    std::vector<sf::RectangleShape> flowerShapes; // เก็บสี่เหลี่ยม (สำรอง)
    int money = 100;
    int seeds = 5;
    sf::Clock deltaClock;

    // 4. ลูปหลักของเกม (Game Loop)
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(window, event);

            if (event.type == sf::Event::Closed)
                window.close();

            // กดคลิกซ้ายเพื่อปลูกดอกไม้
            if (event.type == sf::Event::MouseButtonPressed && 
                event.mouseButton.button == sf::Mouse::Left &&
                !ImGui::GetIO().WantCaptureMouse) { // ต้องไม่คลิกโดนเมนู
                
                if (seeds > 0) {
                    sf::Vector2f mousePos(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
                    
                    if (hasTexture) {
                        sf::Sprite newFlower;
                        newFlower.setTexture(flowerTexture);
                        newFlower.setOrigin(flowerTexture.getSize().x / 2.f, flowerTexture.getSize().y / 2.f);
                        newFlower.setPosition(mousePos);
                        newFlower.setScale(0.2f, 0.2f); // ปรับขนาดรูปตามใจชอบ
                        flowerSprites.push_back(newFlower);
                    } else {
                        sf::RectangleShape newFlower(sf::Vector2f(40.f, 40.f));
                        newFlower.setFillColor(sf::Color::Magenta);
                        newFlower.setOrigin(20.f, 20.f);
                        newFlower.setPosition(mousePos);
                        flowerShapes.push_back(newFlower);
                    }
                    
                    seeds--;
                }
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        // --- สร้างเมนูควบคุม (UI) ---
        ImGui::Begin("Flower Shop Manager"); // ชื่อหัวข้อเมนู
        
        ImGui::Text("Money: $%d", money);
        ImGui::Text("Seeds: %d", seeds);
        
        ImGui::Separator();

        if (ImGui::Button("Buy Seed ($10)")) {
            if (money >= 10) {
                money -= 10;
                seeds++;
            }
        }

        int count = hasTexture ? flowerSprites.size() : flowerShapes.size();
        ImGui::Text("Flowers planted: %d", count);
        
        if (count > 0) {
            if (ImGui::Button("Sell All Flowers (Get $20 each)")) {
                money += count * 20;
                if (hasTexture) flowerSprites.clear();
                else flowerShapes.clear();
            }
        }
        
        ImGui::End();

        // --- วาดหน้าจอ ---
        window.clear(sf::Color(50, 150, 50)); // พื้นหลังสีเขียวหญ้า

        // วาดดอกไม้ทั้งหมด
        if (hasTexture) {
            for (auto& flower : flowerSprites) window.draw(flower);
        } else {
            for (auto& flower : flowerShapes) window.draw(flower);
        }

        ImGui::SFML::Render(window); // วาดเมนูทับ
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}