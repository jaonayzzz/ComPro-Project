#include "../geometry/PetalShapes.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <algorithm>

namespace Dahlia
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
        float w,
        float h,
        sf::Color color,
        float phaseOffset
    )
    {
        for (int i = 0; i < count; ++i)
        {
            float angle = (i * 2.f * 3.1415926f / count) + phaseOffset;
            float deg = angle * 180.f / 3.1415926f;

            sf::ConvexShape petal =
                Petal::sharp(w, h);

            petal.setPosition({
                center.x + std::cos(angle) * radius,
                center.y + std::sin(angle) * radius
            });

            petal.setRotation(sf::degrees(deg + 90.f));
            petal.setFillColor(color);

            window.draw(petal);
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
        // OUTER LAYER
        // ===============================
        drawLayer(
            window,
            pos,
            16,
            R * 0.78f,
            R * 0.30f,
            R * 0.95f,
            darken(baseColor, 25),
            0.f
        );

        // ===============================
        // MID LAYER (offset)
        // ===============================
        drawLayer(
            window,
            pos,
            20,
            R * 0.52f,
            R * 0.25f,
            R * 0.75f,
            baseColor,
            3.1415926f / 20.f
        );

        // ===============================
        // INNER LAYER (tighter + brighter)
        // ===============================
        drawLayer(
            window,
            pos,
            24,
            R * 0.28f,
            R * 0.18f,
            R * 0.55f,
            lighten(baseColor, 20),
            3.1415926f / 24.f
        );

        // ===============================
        // CORE
        // ===============================
        sf::CircleShape center(R * 0.16f, 64);
        center.setOrigin({center.getRadius(), center.getRadius()});
        center.setPosition(pos);
        center.setFillColor(lighten(baseColor, 35));
        window.draw(center);
    }
}