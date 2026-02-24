#include "../geometry/PetalShapes.h"
#include <SFML/Graphics.hpp>
#include <SFML/System/Angle.hpp>
#include <cmath>
#include <algorithm>

namespace Lotus
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

    static void drawRing(
        sf::RenderWindow& window,
        sf::Vector2f center,
        int count,
        float radius,
        float petalW,
        float petalH,
        sf::Color color
    )
    {
        for (int i = 0; i < count; ++i)
        {
            float angle = i * 2.f * 3.1415926f / count;
            float deg = angle * 180.f / 3.1415926f;

            sf::ConvexShape petal =
                Petal::teardrop(petalW, petalH);

            petal.setPosition({
                center.x + std::cos(angle) * radius,
                center.y + std::sin(angle) * radius
            });

            // ✅ SFML 3 rotation
            petal.setRotation(sf::degrees(deg + 90.f));

            petal.setFillColor(color);

            window.draw(petal);

            Petal::addHighlight(
                window,
                petal,
                sf::Color(255, 255, 255, 45),
                0.12f
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

        drawRing(
            window,
            pos,
            8,
            R * 0.75f,
            R * 0.45f,
            R * 1.1f,
            darken(baseColor, 20)
        );

        drawRing(
            window,
            pos,
            10,
            R * 0.45f,
            R * 0.38f,
            R * 0.85f,
            baseColor
        );

        drawRing(
            window,
            pos,
            12,
            R * 0.22f,
            R * 0.25f,
            R * 0.55f,
            lighten(baseColor, 25)
        );

        // ===============================
        // CENTER DISC
        // ===============================
        sf::CircleShape center(R * 0.20f, 48);

        // ✅ setOrigin FIX
        center.setOrigin({ center.getRadius(), center.getRadius() });

        center.setPosition(pos);
        center.setFillColor(sf::Color(255, 220, 150));

        window.draw(center);
    }
}