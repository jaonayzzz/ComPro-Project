#include "../geometry/PetalShapes.h"
#include <SFML/Graphics.hpp>
#include <SFML/System/Angle.hpp>
#include <cmath>
#include <algorithm>

namespace Rose
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

    void draw(
        sf::RenderWindow& window,
        sf::Vector2f pos,
        float size,
        sf::Color baseColor,
        float rotation /* = 0.f */
    )
    {
        const float R = size * 0.5f;
        const int layers = 4;

        for (int layer = 0; layer < layers; ++layer)
        {
            int petals = 5 + layer * 2;
            float radius = R * (0.15f + layer * 0.18f);
            float petalH = R * (0.55f - layer * 0.08f);
            float petalW = R * (0.45f - layer * 0.05f);

            sf::Color layerColor = darken(baseColor, layer * 18);

            for (int i = 0; i < petals; ++i)
            {
                float angle = i * 360.f / petals
                            + layer * 18.f
                            + rotation;

                float rad = angle * 3.1415926f / 180.f;

                sf::ConvexShape petal =
                    Petal::teardrop(petalW, petalH);

                petal.setFillColor(layerColor);

                petal.setPosition({
                    pos.x + std::cos(rad) * radius,
                    pos.y + std::sin(rad) * radius
                });

                // ✅ SFML 3 ต้องใช้ Angle
                petal.setRotation(sf::degrees(angle + 90.f));

                window.draw(petal);

                // highlight
                Petal::addHighlight(
                    window,
                    petal,
                    sf::Color(255, 220, 220, 90),
                    0.18f
                );
            }
        }

        // 🌹 center bud
        sf::CircleShape core(R * 0.18f, 48);

        // ✅ setOrigin ต้องใช้ Vector2f เดียว
        core.setOrigin({ core.getRadius(), core.getRadius() });

        core.setPosition(pos);
        core.setFillColor(darken(baseColor, 40));

        window.draw(core);
    }
}