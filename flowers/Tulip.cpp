#include "../geometry/PetalShapes.h"
#include <SFML/Graphics.hpp>
#include <SFML/System/Angle.hpp>
#include <cmath>
#include <algorithm>

namespace Tulip
{
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
        float size,
        sf::Color baseColor,
        float rotation /* = 0.f */
    )
    {
        const float R = size * 0.5f;

        // ===============================
        // BACK PETALS (2 petals)
        // ===============================
        for (int i = -1; i <= 1; i += 2)
        {
            sf::ConvexShape petal =
                Petal::roundedTeardrop(
                    R * 0.55f,
                    R * 1.05f,
                    0.65f
                );

            petal.setFillColor(darken(baseColor, 25));

            petal.setPosition({
                pos.x + i * R * 0.22f,
                pos.y - R * 0.12f
            });

            // ✅ SFML 3 rotation
            petal.setRotation(sf::degrees(rotation + i * 8.f));

            window.draw(petal);

            Petal::addHighlight(
                window,
                petal,
                sf::Color(255, 255, 255, 60),
                0.12f
            );
        }

        // ===============================
        // FRONT PETALS (3 petals)
        // ===============================
        for (int i = -1; i <= 1; ++i)
        {
            sf::ConvexShape petal =
                Petal::roundedTeardrop(
                    R * 0.6f,
                    R * 0.95f,
                    0.7f
                );

            petal.setFillColor(lighten(baseColor, i == 0 ? 15 : 5));

            petal.setPosition({
                pos.x + i * R * 0.18f,
                pos.y + R * 0.05f
            });

            // ✅ SFML 3 rotation
            petal.setRotation(sf::degrees(rotation + i * 6.f));

            window.draw(petal);

            Petal::addHighlight(
                window,
                petal,
                sf::Color(255, 240, 240, 80),
                0.18f
            );
        }

        // ===============================
        // INNER CORE
        // ===============================
        sf::CircleShape core(R * 0.18f, 40);

        // ✅ setOrigin ต้องใช้ Vector2f เดียว
        core.setOrigin({ core.getRadius(), core.getRadius() });

        core.setPosition({ pos.x, pos.y + R * 0.15f });
        core.setFillColor(darken(baseColor, 35));

        window.draw(core);
    }
}