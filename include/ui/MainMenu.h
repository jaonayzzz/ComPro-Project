#pragma once
#include <SFML/Graphics.hpp>
#include "AppState.h"
#include "DataModels.h"

void drawMainMenu(
    sf::RenderWindow& window,
    AppState& state,
    UserSelection& selection
);