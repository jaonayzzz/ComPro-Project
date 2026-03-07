#include "../geometry/PetalShapes.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <algorithm>

namespace Peony
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

    static sf::Color darken(sf::Color c, int d)
    {
        auto clamp = [](int v){ return std::max(0, std::min(255, v)); };
        return sf::Color(
            clamp(c.r - d),
            clamp(c.g - d),
            clamp(c.b - d),
            c.a
        );
    }

    static void drawLayer(
        sf::RenderWindow& window,
        sf::Vector2f center,
        int count,
        float radius,
        float petalW,
        float petalH,
        float roundness,
        sf::Color color,
        float phase
    )
    {
        for (int i = 0; i < count; ++i)
        {
            float angle = (i * 2.f * 3.1415926f / count) + phase;
            float offset = radius * (0.85f + 0.15f * std::sin(i * 1.7f));

            sf::ConvexShape petal =
                Petal::roundedTeardrop(petalW, petalH, roundness);

            petal.setPosition({
                center.x + std::cos(angle) * offset,
                center.y + std::sin(angle) * offset
            });

            petal.setRotation(sf::degrees(angle * 180.f / 3.1415926f + 90.f));
            petal.setFillColor(color);

            window.draw(petal);

            // soft highlight (peony is very soft)
            Petal::addHighlight(
                window,
                petal,
                sf::Color(255, 255, 255, 50),
                0.15f
            );
        }
    }

    void draw(
        sf::RenderWindow& window,
        sf::Vector2f pos,
        float size,
        sf::Color baseColor
    )
    {
        const float R = size * 0.5f;

        // ===============================
        // OUTER LAYER (large & loose)
        // ===============================
        drawLayer(
            window,
            pos,
            12,
            R * 0.85f,
            R * 0.55f,
            R * 0.9f,
            0.7f,
            darken(baseColor, 15),
            0.3f
        );

        // ===============================
        // MID LAYER (dense volume)
        // ===============================
        drawLayer(
            window,
            pos,
            16,
            R * 0.55f,
            R * 0.45f,
            R * 0.75f,
            0.75f,
            baseColor,
            1.1f
        );

        // ===============================
        // INNER LAYER (tight core)
        // ===============================
        drawLayer(
            window,
            pos,
            18,
            R * 0.28f,
            R * 0.32f,
            R * 0.55f,
            0.8f,
            lighten(baseColor, 20),
            2.4f
        );

        // ===============================
        // CORE MASS (no hard center)
        // ===============================
        sf::CircleShape core(R * 0.18f, 48);
        core.setOrigin({ core.getRadius(), core.getRadius() });
        core.setPosition(pos);
        core.setFillColor(lighten(baseColor, 35));
        window.draw(core);
    }
}