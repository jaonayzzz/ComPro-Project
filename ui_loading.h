#pragma once
#include <SFML/Graphics.hpp>
#include "app_state.h"

void initLoadingScreen();
void updateLoadingScreen(AppState& state);
void drawLoadingScreen(sf::RenderWindow& window);