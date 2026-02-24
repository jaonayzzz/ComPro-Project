#include "../geometry/PetalShapes.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <algorithm>

namespace Hydrangea
{
    static sf::Color soften(sf::Color c, int d)
    {
        auto clamp = [](int v){ return std::max(0, std::min(255, v)); };
        return sf::Color(
            clamp(c.r + d),
            clamp(c.g + d),
            clamp(c.b + d),
            c.a
        );
    }

    // ===============================
    // Single flowerlet (4 petals)
    // ===============================
    static void drawFlowerlet(
        sf::RenderWindow& window,
        sf::Vector2f pos,
        float size,
        sf::Color base
    )
    {
        float r = size * 0.5f;

        for (int i = 0; i < 4; ++i)
        {
            float angleDeg = i * 90.f;

            sf::ConvexShape petal =
            Petal::roundedTeardrop(
            r * 0.55f,
            r * 0.75f,
            0.6f
            );

            petal.setPosition(pos);
            petal.setRotation(sf::degrees(angleDeg));
            petal.setFillColor(base);

            window.draw(petal);
        }

        // tiny center
        sf::CircleShape c(r * 0.18f, 24);
        c.setOrigin({c.getRadius(), c.getRadius()});
        c.setPosition(pos);
        c.setFillColor(soften(base, -20));
        window.draw(c);
    }

    // ===============================
    // Main cluster
    // ===============================
    void draw(
        sf::RenderWindow& window,
        sf::Vector2f pos,
        float size
    )
    {
        const float R = size * 0.5f;

        sf::Color base (130,170,255);
        sf::Color mid  (155,195,255);
        sf::Color light(185,215,255);

        struct Ring { int count; float radius; sf::Color color; };

        std::vector<Ring> rings =
        {
            { 6,  R * 0.18f, base  },
            { 10, R * 0.35f, mid   },
            { 14, R * 0.52f, light }
        };

        float flowerletSize = R * 0.28f;

        for (auto& ring : rings)
        {
            for (int i = 0; i < ring.count; ++i)
            {
                float angle = i * 2.f * 3.1415926f / ring.count;

                sf::Vector2f p{
                    pos.x + std::cos(angle) * ring.radius,
                    pos.y + std::sin(angle) * ring.radius
                };

                drawFlowerlet(
                    window,
                    p,
                    flowerletSize,
                    ring.color
                );
            }
        }

        drawFlowerlet(
            window,
            pos,
            flowerletSize * 1.1f,
            soften(mid, -10)
        );
    }
}