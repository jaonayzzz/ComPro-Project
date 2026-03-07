#pragma once
#include "../geometry/PetalShapes.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <algorithm>

namespace Chamomile
{
    static sf::Color lighten(sf::Color c, int d)
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

        sf::Color petalColor(250, 250, 240);
        sf::Color centerOuter(255, 205, 90);
        sf::Color centerInner(255, 175, 60);

        // ===============================
        // PETALS (SLIGHTLY DROOPING)
        // ===============================
        int count = 22;

        for (int i = 0; i < count; ++i)
        {
            float angle = i * 2.f * 3.1415926f / count;
            float deg   = angle * 180.f / 3.1415926f;

            sf::ConvexShape petal =
                Petal::teardrop(R * 0.18f, R * 1.1f);

            petal.setPosition({
                pos.x + std::cos(angle) * R * 0.55f,
                pos.y + std::sin(angle) * R * 0.55f
            });
            petal.setRotation(sf::degrees(deg + 95.f));
            petal.setFillColor(petalColor);

            window.draw(petal);

            // soft highlight
            Petal::addHighlight(
                window,
                petal,
                sf::Color(255, 255, 255, 50),
                0.18f
            );
        }

        // ===============================
        // CENTER (DOMED)
        // ===============================
        sf::CircleShape outer(R * 0.32f, 64);
        outer.setOrigin({ outer.getRadius(), outer.getRadius() });
        outer.setPosition(pos);
        outer.setFillColor(centerOuter);
        window.draw(outer);

        sf::CircleShape inner(R * 0.22f, 64);
        inner.setOrigin({ inner.getRadius(), inner.getRadius() });
        inner.setPosition(pos);
        inner.setFillColor(centerInner);
        window.draw(inner);

        // highlight dot
        sf::CircleShape highlight(R * 0.10f, 32);
        highlight.setOrigin({ highlight.getRadius(), highlight.getRadius() });
        highlight.setPosition({
            pos.x - R * 0.08f,
            pos.y - R * 0.08f
        });
        highlight.setFillColor(lighten(centerOuter, 20));
        window.draw(highlight);
    }
}