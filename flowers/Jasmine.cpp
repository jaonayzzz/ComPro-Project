#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <algorithm>

namespace Jasmine
{
    static sf::Color soften(sf::Color c, int d)
    {
        auto clamp = [](int v){ return std::max(0, std::min(255, v)); };
        return sf::Color(clamp(c.r + d), clamp(c.g + d), clamp(c.b + d), c.a);
    }

    void draw(sf::RenderWindow& window, sf::Vector2f pos, float size)
    {
        const float R = size * 0.5f;

        sf::Color white(250,250,245);
        sf::Color cream(245,240,220);
        sf::Color centerYellow(245,215,140);

        int petals = 6;
        float offset = R * 0.35f;

        // ===============================
        // PETALS
        // ===============================
        for (int i = 0; i < petals; ++i)
        {
            float angleDeg = i * 360.f / petals;
            float angleRad = angleDeg * 3.1415926f / 180.f;

            sf::CircleShape petal(R * 0.32f, 64);
            petal.setOrigin({petal.getRadius(), petal.getRadius()});
            petal.setPosition({
                pos.x + std::cos(angleRad) * offset,
                pos.y + std::sin(angleRad) * offset
            });
            petal.setRotation(sf::degrees(angleDeg));
            petal.setScale({0.6f, 1.5f});
            petal.setFillColor((i % 2 == 0) ? white : soften(white, -6));

            window.draw(petal);
        }

        // ===============================
        // INNER LAYER
        // ===============================
        for (int i = 0; i < petals; ++i)
        {
            float angleDeg = i * 360.f / petals + 20.f;
            float angleRad = angleDeg * 3.1415926f / 180.f;

            sf::CircleShape petal(R * 0.24f, 48);
            petal.setOrigin({petal.getRadius(), petal.getRadius()});
            petal.setPosition({
                pos.x + std::cos(angleRad) * R * 0.22f,
                pos.y + std::sin(angleRad) * R * 0.22f
            });
            petal.setRotation(sf::degrees(angleDeg));
            petal.setScale({0.55f, 1.3f});
            petal.setFillColor(cream);

            window.draw(petal);
        }

        // ===============================
        // CENTER
        // ===============================
        sf::CircleShape center(R * 0.16f, 48);
        center.setOrigin({center.getRadius(), center.getRadius()});
        center.setPosition(pos);
        center.setFillColor(centerYellow);
        window.draw(center);

        sf::CircleShape core(R * 0.08f, 32);
        core.setOrigin({core.getRadius(), core.getRadius()});
        core.setPosition(pos);
        core.setFillColor(sf::Color(235,200,120));
        window.draw(core);
    }
}