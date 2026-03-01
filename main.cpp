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
#include "ui_preset_page.h"
#include "confirm.cpp"

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
    std::vector<sf::Texture> flowerTextures;
    static std::vector<std::string> fileNames = {
        "picture/Lotus.png", "picture/White-Carnation.png", "picture/White-Chrysanthemum.png",
        "picture/White-Rose.png", "picture/White-Orchid.png", "picture/White-Tulip.png",
        "picture/Red-Poppy.png", "picture/White-Lily.png", "picture/White-Hydrangea.png",
        "picture/White-Anemone.png", "picture/Marigold.png", "picture/White-Daisy.png",
        "picture/Yellow-Daisy.png", "picture/Sunflower.png", "picture/Pink-Carnation.png",
        "picture/Yellow-Chrysanthemum.png", "picture/Yellow-Rose.png", "picture/Purple-Dahlia.png",
        "picture/Purple-Orchid.png", "picture/Yellow-Tulip.png", "picture/Purple-Iris.png",
        "picture/Red-Peony.png", "picture/Jasmine.png", "picture/Red-Carnation.png",
        "picture/Red-Rose.png", "picture/Pink-Rose.png", "picture/Red-Dahlia.png",
        "picture/Lavender.png", "picture/Chamomile.png", "picture/Red-Tulip.png",
        "picture/Forget-Me-Not.png", "picture/Pink-Lily.png", "picture/Blue-Hydrangea.png",
        "picture/Pink-Hydrangea.png", "picture/Red-Anemone.png", "picture/Pink-Peony.png"
    };
    std::vector<Flower> selectedFlowers;
    initStartScreen();

    //โหลดรูปภาพ
    for (auto& flower : flowerList) {
    // โหลดรูปโดยใช้ path ที่ระบุไว้ใน flower.imagePath
    if (!flower.texture.loadFromFile(flower.imagePath)) {
        // ถ้าโหลดไม่ได้ (เช่น พิมพ์ path ผิด หรือหาไฟล์ไม่เจอ)
        std::cerr << "Could not load image: " << flower.imagePath << std::endl;
    }else {
            // สำคัญ: เซ็ตให้ Texture ทำงานราบรื่น (Smooth)
            flower.texture.setSmooth(true);
        }
}
        
    for (const auto& path : fileNames) {
        sf::Texture tex;
        if (tex.loadFromFile(path)) {
            tex.setSmooth(true);
            flowerTextures.push_back(tex); // ดันรูปเก็บเข้าตะกร้า
        } else {
            std::cerr << "Could not load image: " << path << std::endl;
            // ใส่ Texture เปล่าๆ เข้าไปกันพัง เผื่อไฟล์ภาพหาย
            flowerTextures.push_back(sf::Texture()); 
        }
    }
    sf::Clock deltaClock;
    AppState currentState = AppState::START_SCREEN;
    UserSelection userSelection;
    OrderCardData Cardsdata;
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
    Custom_Pages(flowerList, selectedFlowers, currentState,userSelection); 
        break;

    case AppState::RANDOM_MODE:
        Random_Pages(window, currentState, userSelection);
        /*ImGui::Begin("Random Mode");
        ImGui::Text("Random Mode is not implemented yet.");
        if (ImGui::Button("Back"))
            currentState = AppState::MAIN_MENU;
        ImGui::End();*/
        break;

    case AppState::PRESET_PAGE:
        drawPresetPage(window, currentState, userSelection);
        break;

    case AppState::SUMMARY: // เพิ่ม case นี้
        drawSummaryPage(window,currentState, userSelection,flowerTextures);
        break;
    case AppState::CARD:
        card(userSelection.selectedFlowers,currentState,userSelection.occasion);
        break;
    case AppState::CONFIRM:
        confirm(userSelection.selectedFlowers,Cardsdata.message,window,userSelection,flowerTextures,Cardsdata);
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

