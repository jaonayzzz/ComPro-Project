#pragma once
#include <SFML/Graphics.hpp>
#include "data_models.h"

class FlowerRenderer
{
public:
    static void draw(
        sf::RenderWindow& window,
        FlowerType type,
        sf::Vector2f position,
        float size,
        float rotation = 0.f
    );

private:
    // Basic families
    static void drawDaisy(sf::RenderWindow&, sf::Vector2f, float, float, sf::Color, sf::Color);
    static void drawRose(sf::RenderWindow&, sf::Vector2f, float, float, sf::Color);
    static void drawTulip(sf::RenderWindow&, sf::Vector2f, float, float, sf::Color);
    static void drawHydrangea(sf::RenderWindow&, sf::Vector2f, float, sf::Color);

    // Special
    static void drawLotus(sf::RenderWindow&, sf::Vector2f, float, float);
    static void drawSunflower(sf::RenderWindow&, sf::Vector2f, float, float);
    static void drawCarnation(sf::RenderWindow&, sf::Vector2f, float, float, sf::Color);
    static void drawOrchid(sf::RenderWindow&, sf::Vector2f, float, float, sf::Color);
};