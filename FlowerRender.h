#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "data_models.h" // เพื่อให้รู้จัก UserSelection และ Flower

void renderBouquet(sf::RenderWindow& window, const UserSelection& selection, const std::vector<sf::Texture>& flowerTextures);