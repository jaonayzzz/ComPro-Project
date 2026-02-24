#include "../geometry/PetalShapes.h"
#include <SFML/Graphics.hpp>
#include <SFML/System/Angle.hpp>
#include <cmath>
#include <algorithm>

namespace Sunflower
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
            float deg = angle * 180.f / 3.1415926f;

            sf::ConvexShape petal =
                Petal::sharp(w, h);

            petal.setPosition({
                center.x + std::cos(angle) * radius,
                center.y + std::sin(angle) * radius
            });

            // ✅ SFML 3 ต้องใช้ Angle
            petal.setRotation(sf::degrees(deg + 90.f));

            petal.setFillColor(color);

            window.draw(petal);
        }
    }

    void draw(
        sf::RenderWindow& window,
        sf::Vector2f pos,
        float size
    )
    {
        const float R = size * 0.5f;

        sf::Color petalColor(255, 185, 40);
        sf::Color darkSeed(90, 55, 20);
        sf::Color midSeed(130, 80, 35);

        // ===============================
        // OUTER RAY PETALS
        // ===============================
        drawPetalRing(
            window,
            pos,
            22,
            R * 0.78f,
            R * 0.28f,
            R * 1.05f,
            petalColor
        );

        // ===============================
        // DARK SEED DISK
        // ===============================
        sf::CircleShape outerSeed(R * 0.42f, 64);

        // ✅ setOrigin ต้องส่ง Vector2f ตัวเดียว
        outerSeed.setOrigin({ outerSeed.getRadius(), outerSeed.getRadius() });

        outerSeed.setPosition(pos);
        outerSeed.setFillColor(darkSeed);
        window.draw(outerSeed);

        // ===============================
        // MID SEED DISK
        // ===============================
        sf::CircleShape innerSeed(R * 0.32f, 64);
        innerSeed.setOrigin({ innerSeed.getRadius(), innerSeed.getRadius() });
        innerSeed.setPosition(pos);
        innerSeed.setFillColor(midSeed);
        window.draw(innerSeed);

        // ===============================
        // CENTER HIGHLIGHT
        // ===============================
        sf::CircleShape core(R * 0.18f, 64);
        core.setOrigin({ core.getRadius(), core.getRadius() });
        core.setPosition(pos);
        core.setFillColor(lighten(midSeed, 20));
        window.draw(core);
    }
}