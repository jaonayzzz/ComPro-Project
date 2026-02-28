#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include "app_state.h"
#include "data_models.h"
#include "ui_mainmenu.h"
#include "config.h"
#include "Custom_Pages.h"
#include "Random_Pages.cpp"
#include "font_manager.h"
#include <iostream>
#include <optional>
#include "Summary_Page.cpp"
#include "FlowerRender.cpp"
#include "ui_start.h"
#include "card.cpp"

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
    std::vector<Flower> selectedFlowers;
    initStartScreen();

    //โหลดรูปภาพ
    for (auto& flower : flowerList) {
    // โหลดรูปโดยใช้ path ที่ระบุไว้ใน flower.imagePath
    if (!flower.texture.loadFromFile(flower.imagePath)) {
        // ถ้าโหลดไม่ได้ (เช่น พิมพ์ path ผิด หรือหาไฟล์ไม่เจอ)
        std::cerr << "Could not load image: " << flower.imagePath << std::endl;
    }
}
    
    sf::Clock deltaClock;
    AppState currentState = AppState::START_SCREEN;
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
    window.clear(sf::Color(245, 245, 245));

    switch (currentState)
    {
    case AppState::START_SCREEN:
        drawStartScreen(window, currentState);
        break;
        
    case AppState::MAIN_MENU:
        drawMainMenu(window, currentState, userSelection);
        break;

    case AppState::CUSTOM_MODE:
    Custom_Pages(flowerList, selectedFlowers, currentState); 
        break;

    case AppState::RANDOM_MODE:
        Random_Pages(window, currentState, userSelection);
        /*ImGui::Begin("Random Mode");
        ImGui::Text("Random Mode is not implemented yet.");
        if (ImGui::Button("Back"))
            currentState = AppState::MAIN_MENU;
        ImGui::End();*/
        break;

    case AppState::SUMMARY: // เพิ่ม case นี้
        drawSummaryPage(window,currentState, userSelection,flowerTextures);
        break;
    case AppState::CARD:
        card(userSelection.selectedFlowers,currentState,userSelection.occasion);
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

