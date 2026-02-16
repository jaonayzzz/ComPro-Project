#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <optional> // เพิ่มเข้ามาสำหรับ SFML 3

enum CurrentScene {
    SCENE_START_MENU,
    SCENE_GAME_PLAY
};

int main() {
    // 1. สร้างหน้าต่าง (SFML 3)
    sf::RenderWindow window(sf::VideoMode({800, 600}), "My Investment App");
    window.setFramerateLimit(60);

    // 2. เริ่มต้น ImGui-SFML
    if (!ImGui::SFML::Init(window)) return -1;

    // 3. โหลดรูปดาว (ใช้ flower.png)
    sf::Texture starTexture;
    if (!starTexture.loadFromFile("flower.png")) return -1;
    
    sf::Sprite starSprite(starTexture);
    starSprite.setScale({0.5f, 0.5f});
    
    // จัดกลางจอ
    sf::FloatRect bounds = starSprite.getGlobalBounds();
    starSprite.setPosition({(800.0f - bounds.size.x) / 2.0f, (600.0f - bounds.size.y) / 2.0f});

    CurrentScene currentScene = SCENE_START_MENU;
    sf::Clock deltaClock;

    // 4. Game Loop
    while (window.isOpen()) {
        // --- ส่วนที่แก้สำหรับ SFML 3 ---
        while (const std::optional event = window.pollEvent()) {
            ImGui::SFML::ProcessEvent(window, *event);
            
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }
        // ----------------------------

        ImGui::SFML::Update(window, deltaClock.restart());

        if (currentScene == SCENE_START_MENU) {
            ImGui::SetNextWindowPos(ImVec2(250, 250), ImGuiCond_Always);
            ImGui::SetNextWindowSize(ImVec2(300, 100), ImGuiCond_Always);
            ImGui::Begin("Start Menu", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
            
            ImGui::Text("Welcome to My Investment Planner!");
            if (ImGui::Button("START", ImVec2(100, 30))) {
                currentScene = SCENE_GAME_PLAY;
            }
            ImGui::End();
        } 
        else if (currentScene == SCENE_GAME_PLAY) {
            ImGui::Begin("Info");
            ImGui::Text("You've reached the Star Page!");
            if (ImGui::Button("Back")) {
                currentScene = SCENE_START_MENU;
            }
            ImGui::End();
        }

        window.clear(sf::Color(45, 45, 45));

        if (currentScene == SCENE_GAME_PLAY) {
            window.draw(starSprite);
        }

        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}