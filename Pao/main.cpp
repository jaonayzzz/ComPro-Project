#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

#include "app_state.h"
#include "data_models.h"
#include "ui_mainmenu.h"
#include "config.h"

#include <iostream>

int main() {
    // ===============================
    // Create Window
    // ===============================
    sf::RenderWindow window(
    sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
    WINDOW_TITLE
);
window.setFramerateLimit(TARGET_FPS);

    // ===============================
    // Init ImGui-SFML
    // ===============================
    if (!ImGui::SFML::Init(window)) {
        std::cerr << "Failed to init ImGui-SFML\n";
        return -1;
    }

    sf::Clock deltaClock;

    // ===============================
    // App State & User Data
    // ===============================
    AppState currentState = AppState::MAIN_MENU;
    UserSelection userSelection;

    // default values
    userSelection.budget = 500;
    userSelection.isCustomMode = true;

    // ===============================
    // Main Loop
    // ===============================
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed) {
                currentState = AppState::EXIT;
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        // ===============================
        // Draw UI by State
        // ===============================
        switch (currentState) {
            case AppState::MAIN_MENU:
                drawMainMenu(currentState, userSelection);
                break;

            case AppState::CUSTOM_MODE:
                // TODO: เรียก UI ของทีม Custom Mode
                ImGui::Begin("Custom Mode");
                ImGui::Text("Custom Mode is not implemented yet.");
                if (ImGui::Button("Back to Main Menu")) {
                    currentState = AppState::MAIN_MENU;
                }
                ImGui::End();
                break;

            case AppState::RANDOM_MODE:
                // TODO: เรียก UI ของทีม Random Mode
                ImGui::Begin("Random Mode");
                ImGui::Text("Random Mode is not implemented yet.");
                if (ImGui::Button("Back to Main Menu")) {
                    currentState = AppState::MAIN_MENU;
                }
                ImGui::End();
                break;

            case AppState::EXIT:
                window.close();
                break;
        }

        // ===============================
        // Render
        // ===============================
        window.clear(sf::Color(245, 245, 245));
        ImGui::SFML::Render(window);
        window.display();
    }

    // ===============================
    // Cleanup
    // ===============================
    ImGui::SFML::Shutdown();
    return 0;
}