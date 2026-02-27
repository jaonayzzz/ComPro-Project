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

void loadpicture(vector <string> filenames){
    for (const auto& file : fileNames) { 
        sf::Texture temp;
        temp.loadFromFile(file);
        flowerTextures.push_back(temp);
    }
}
    