#pragma once
#include <SFML/Graphics.hpp>
#include "app_state.h"
#include "data_models.h"

void drawShop(
    sf::RenderWindow& window,
    AppState& state,
    UserSelection& selection
);