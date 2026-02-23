#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

#include "app_state.h"
#include "data_models.h"
#include "ui_mainmenu.h"
#include "config.h"
#include "Custom_Pages.h"
#include "font_manager.h"

#include <iostream>
#include <optional>

int main()
{
    sf::RenderWindow window(
        sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}),
        WINDOW_TITLE
    );
    window.setFramerateLimit(TARGET_FPS);

    if (!ImGui::SFML::Init(window))
    {
        std::cerr << "Failed to init ImGui-SFML\n";
        return -1;
    }

    // ⭐ โหลดฟอนต์ (ครั้งเดียว)
    LoadFonts();

    sf::Clock deltaClock;

    AppState currentState = AppState::MAIN_MENU;
    UserSelection userSelection;
    userSelection.isCustomMode = true;

    while (window.isOpen())
{
    while (const std::optional<sf::Event> event = window.pollEvent())
    {
        ImGui::SFML::ProcessEvent(window, *event);
        if (event->is<sf::Event::Closed>())
            window.close();
    }

    ImGui::SFML::Update(window, deltaClock.restart());

    // ⭐ ต้อง clear ก่อนวาดทุกอย่าง
    window.clear(sf::Color(245, 245, 245));

    switch (currentState)
    {
    case AppState::MAIN_MENU:
        drawMainMenu(window, currentState, userSelection);
        break;

    case AppState::CUSTOM_MODE:
        Custom_Pages();
        break;

    case AppState::RANDOM_MODE:
        ImGui::Begin("Random Mode");
        ImGui::Text("Random Mode is not implemented yet.");
        if (ImGui::Button("Back"))
            currentState = AppState::MAIN_MENU;
        ImGui::End();
        break;

    case AppState::EXIT:
        window.close();
        break;
    }

    ImGui::SFML::Render(window);
    window.display();
}

    ImGui::SFML::Shutdown();
    return 0;
}