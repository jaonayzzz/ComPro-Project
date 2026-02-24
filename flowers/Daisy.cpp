#include "../geometry/PetalShapes.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <algorithm>

namespace Daisy
{
    static void drawPetalRing(
        sf::RenderWindow& window,
        sf::Vector2f center,
        int count,
        float radius,
        float w,
        float h,
        sf::Color color
    )
    {
        for (int i = 0; i < count; ++i)
        {
            float angle = i * 2.f * 3.1415926f / count;
            float deg   = angle * 180.f / 3.1415926f;

            sf::ConvexShape petal = Petal::teardrop(w, h);

            petal.setPosition({
                center.x + std::cos(angle) * radius,
                center.y + std::sin(angle) * radius
            });

            petal.setRotation(sf::degrees(deg + 90.f));
            petal.setFillColor(color);

            window.draw(petal);

            Petal::addHighlight(
                window,
                petal,
                sf::Color(255,255,255,55),
                0.15f
            );
        }
    }

    void draw(
        sf::RenderWindow& window,
        sf::Vector2f pos,
        float size,
        sf::Color petalColor,
        sf::Color centerColor
    )
    {
        const float R = size * 0.5f;

        drawPetalRing(
            window,
            pos,
            18,
            R * 0.62f,
            R * 0.28f,
            R * 1.05f,
            petalColor
        );

        // CENTER DISC
        sf::CircleShape center(R * 0.32f, 64);
        center.setOrigin({center.getRadius(), center.getRadius()});
        center.setPosition(pos);
        center.setFillColor(centerColor);
        window.draw(center);

        // CENTER GRAIN
        sf::CircleShape grain(R * 0.22f, 64);
        grain.setOrigin({grain.getRadius(), grain.getRadius()});
        grain.setPosition(pos);
        grain.setFillColor(sf::Color(
            std::max(0, centerColor.r - 10),
            std::max(0, centerColor.g - 10),
            std::max(0, centerColor.b - 10)
        ));
        window.draw(grain);
    }
}