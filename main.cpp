#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include "AppState.h"
#include "DataModels.h"
#include "ui/MainMenu.h"
#include "Config.h"
#include "ui/Custom.h"
#include "src/ui/Random.cpp"
#include "core/FontManager.h"
#include <iostream>
#include <optional>
#include "src/ui/Summary.cpp"
#include "src/render/FlowerRender.cpp"
#include "ui/Start.h"
#include "ui/Card.h"
#include "ui/Preset.h"
#include "ui/Confirm.h"

int main()
{
    sf::RenderWindow window(
        sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}),
        WINDOW_TITLE,sf::Style::Titlebar | sf::Style::Close 
    );
    window.setFramerateLimit(TARGET_FPS);

    if (!ImGui::SFML::Init(window))
    {
        std::cerr << "Failed to init ImGui-SFML\n";
        return -1;
    }

    LoadFonts();
    std::vector<sf::Texture> flowerTextures;
    static std::vector<std::string> fileNames = {
        "../assets/picture/Lotus.png", "../assets/picture/White-Carnation.png", "../assets/picture/White-Chrysanthemum.png",
        "../assets/picture/White-Rose.png", "../assets/picture/White-Orchid.png", "../assets/picture/White-Tulip.png",
        "../assets/picture/Red-Poppy.png", "../assets/picture/White-Lily.png", "../assets/picture/White-Hydrangea.png",
        "../assets/picture/White-Anemone.png", "../assets/picture/Marigold.png", "../assets/picture/White-Daisy.png",
        "../assets/picture/Yellow-Daisy.png", "../assets/picture/Sunflower.png", "../assets/picture/Pink-Carnation.png",
        "../assets/picture/Yellow-Chrysanthemum.png", "../assets/picture/Yellow-Rose.png", "../assets/picture/Purple-Dahlia.png",
        "../assets/picture/Purple-Orchid.png", "../assets/picture/Yellow-Tulip.png", "../assets/picture/Purple-Iris.png",
        "../assets/picture/Red-Peony.png", "../assets/picture/Jasmine.png", "../assets/picture/Red-Carnation.png",
        "../assets/picture/Red-Rose.png", "../assets/picture/Pink-Rose.png", "../assets/picture/Red-Dahlia.png",
        "../assets/picture/Lavender.png", "../assets/picture/Chamomile.png", "../assets/picture/Red-Tulip.png",
        "../assets/picture/Forget-Me-Not.png", "../assets/picture/Pink-Lily.png", "../assets/picture/Blue-Hydrangea.png",
        "../assets/picture/Pink-Hydrangea.png", "../assets/picture/Red-Anemone.png", "../assets/picture/Pink-Peony.png"
    };
    std::vector<Flower> selectedFlowers;
    initStartScreen();

    for (auto& flower : flowerList) {
    if (!flower.texture.loadFromFile(flower.imagePath)) {
        std::cerr << "Could not load image: " << flower.imagePath << std::endl;
    }else {
            flower.texture.setSmooth(true);
        }
    }
        
    for (const auto& path : fileNames) {
        sf::Texture tex;
        if (tex.loadFromFile(path)) {
            tex.setSmooth(true);
            flowerTextures.push_back(tex);
        } else {
            std::cerr << "Could not load image: " << path << std::endl;
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
        userSelection = UserSelection();
        selectedFlowers.clear();
        Cardsdata.Clear();
        drawMainMenu(window, currentState, userSelection);
        break;

    case AppState::CUSTOM_MODE:
    Custom_Pages(flowerList, selectedFlowers, currentState,userSelection); 
        break;

    case AppState::RANDOM_MODE:
        Random_Pages(window, currentState, userSelection);
        break;

    case AppState::PRESET_PAGE:
        drawPresetPage(window, currentState, userSelection);
        break;

    case AppState::SUMMARY:
        drawSummaryPage(window,currentState, userSelection,flowerTextures);
        break;
    case AppState::CARD:
        card(userSelection,currentState);
        break;
    case AppState::CONFIRM:
        confirm(userSelection.selectedFlowers,Cardsdata.message,window,userSelection,flowerTextures,Cardsdata,containerList,currentState);
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
