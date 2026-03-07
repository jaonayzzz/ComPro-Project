#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <algorithm>

namespace Poppy
{
    static sf::Color shade(sf::Color c, int d)
    {
        auto clamp = [](int v){ return std::max(0, std::min(255, v)); };
        return sf::Color(clamp(c.r + d), clamp(c.g + d), clamp(c.b + d), c.a);
    }

    void draw(sf::RenderWindow& window, sf::Vector2f pos, float size)
    {
        const float R = size * 0.5f;

        sf::Color red(220,40,50);
        sf::Color darkRed(180,20,35);
        sf::Color black(30,30,30);
        sf::Color stamen(240,200,60);

        // PETALS
        for (int i = 0; i < 4; ++i)
        {
            float angle = i * 90.f + 20.f;

            sf::CircleShape petal(R * 0.65f, 64);
            petal.setOrigin({petal.getRadius(), petal.getRadius()});
            petal.setPosition(pos);
            petal.setScale({1.1f, 0.9f});
            petal.setRotation(sf::degrees(angle));
            petal.setFillColor((i % 2 == 0) ? red : shade(red, -15));

            window.draw(petal);
        }

        sf::CircleShape base(R * 0.30f, 64);
        base.setOrigin({base.getRadius(), base.getRadius()});
        base.setPosition(pos);
        base.setFillColor(darkRed);
        window.draw(base);

        sf::CircleShape seed(R * 0.22f, 64);
        seed.setOrigin({seed.getRadius(), seed.getRadius()});
        seed.setPosition(pos);
        seed.setFillColor(black);
        window.draw(seed);

        int stamens = 18;
        for (int i = 0; i < stamens; ++i)
        {
            float angle = i * 2.f * 3.1415926f / stamens;

            sf::CircleShape dot(R * 0.05f, 16);
            dot.setOrigin({dot.getRadius(), dot.getRadius()});
            dot.setPosition({
                pos.x + std::cos(angle) * R * 0.28f,
                pos.y + std::sin(angle) * R * 0.28f
            });
            dot.setFillColor(stamen);
            window.draw(dot);
        }

        sf::CircleShape center(R * 0.07f, 16);
        center.setOrigin({center.getRadius(), center.getRadius()});
        center.setPosition(pos);
        center.setFillColor(stamen);
        window.draw(center);
    }
}