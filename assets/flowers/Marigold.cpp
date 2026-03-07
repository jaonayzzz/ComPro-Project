#pragma once
#include "../geometry/PetalShapes.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <algorithm>

namespace Marigold
{
    static sf::Color shade(sf::Color c, int d)
    {
        auto clamp = [](int v){ return std::max(0, std::min(255, v)); };
        return sf::Color(
            clamp(c.r + d),
            clamp(c.g + d),
            clamp(c.b + d),
            c.a
        );
    }

    void draw(
        sf::RenderWindow& window,
        sf::Vector2f pos,
        float size
    )
    {
        const float R = size * 0.5f;

        sf::Color base(255, 165, 40);
        sf::Color light(255, 200, 80);

        // ===============================
        // OUTER FLUFF LAYERS
        // ===============================
        for (int layer = 0; layer < 4; ++layer)
        {
            int petals = 18 + layer * 6;
            float radius = R * (0.75f - layer * 0.12f);

            for (int i = 0; i < petals; ++i)
            {
                float angle = i * 2.f * 3.1415926f / petals;
                float deg   = angle * 180.f / 3.1415926f;

                sf::ConvexShape petal =
                    Petal::roundedTeardrop(
                        R * 0.22f,
                        R * 0.35f,
                        0.55f
                    );

                petal.setPosition({
                    pos.x + std::cos(angle) * radius,
                    pos.y + std::sin(angle) * radius
                });
                petal.setRotation(sf::degrees(deg + 90.f));
                petal.setFillColor(shade(base, -layer * 8));

                window.draw(petal);
            }
        }

        // ===============================
        // CORE FLUFF (DENSE)
        // ===============================
        for (int i = 0; i < 36; ++i)
        {
            float angle = i * 2.f * 3.1415926f / 36;
            float offset = R * 0.18f;

            sf::CircleShape puff(R * 0.16f, 32);
            puff.setOrigin({ puff.getRadius(), puff.getRadius() });
            puff.setPosition({
                pos.x + std::cos(angle) * offset,
                pos.y + std::sin(angle) * offset
            });
            puff.setFillColor(light);

            window.draw(puff);
        }

        // ===============================
        // CENTER MASS
        // ===============================
        sf::CircleShape core(R * 0.30f, 64);
        core.setOrigin({ core.getRadius(), core.getRadius() });
        core.setPosition(pos);
        core.setFillColor(shade(base, -10));
        window.draw(core);

        // highlight
        sf::CircleShape highlight(R * 0.12f, 32);
        highlight.setOrigin({ highlight.getRadius(), highlight.getRadius() });
        highlight.setPosition({
            pos.x - R * 0.12f,
            pos.y - R * 0.12f
        });
        highlight.setFillColor(light);

        window.draw(highlight);
    }
}