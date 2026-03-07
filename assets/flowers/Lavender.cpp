#include <SFML/Graphics.hpp>
#include <cmath>
#include <algorithm>

namespace Lavender
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

    void draw(
        sf::RenderWindow& window,
        sf::Vector2f pos,
        float size
    )
    {
        const float H = size;
        const float W = size * 0.25f;

        sf::Color base(160, 110, 230);
        sf::Color mid (140, 90, 210);
        sf::Color light(190, 150, 255);

        // ===============================
        // STEM
        // ===============================
        sf::RectangleShape stem({ size * 0.04f, H * 0.9f });

        stem.setOrigin({ stem.getSize().x / 2.f, H * 0.1f });
        stem.setPosition(pos);
        stem.setFillColor(sf::Color(80, 140, 80));

        window.draw(stem);

        // ===============================
        // SPIKE CLUSTER
        // ===============================
        int rows = 14;
        float spacing = H * 0.045f;

        for (int i = 0; i < rows; ++i)
        {
            float y = pos.y - i * spacing;

            float scaleFactor = 1.f - i * 0.035f;
            float width = W * scaleFactor;

            sf::Color rowColor =
                (i < rows / 3) ? light :
                (i < rows * 2 / 3) ? base :
                mid;

            // -----------------
            // LEFT BUD
            // -----------------
            sf::CircleShape left(width * 0.45f, 20);

            left.setOrigin({ left.getRadius(), left.getRadius() });
            left.setPosition({ pos.x - width * 0.3f, y });
            left.setScale({ 1.f, 0.6f });
            left.setFillColor(rowColor);

            window.draw(left);

            // -----------------
            // RIGHT BUD
            // -----------------
            sf::CircleShape right(width * 0.45f, 20);

            right.setOrigin({ right.getRadius(), right.getRadius() });
            right.setPosition({ pos.x + width * 0.3f, y });
            right.setScale({ 1.f, 0.6f });
            right.setFillColor(rowColor);

            window.draw(right);

            // -----------------
            // CENTER BUD
            // -----------------
            sf::CircleShape center(width * 0.5f, 20);

            center.setOrigin({ center.getRadius(), center.getRadius() });
            center.setPosition({ pos.x, y - spacing * 0.15f });
            center.setScale({ 1.f, 0.65f });
            center.setFillColor(rowColor);

            window.draw(center);
        }

        // ===============================
        // TOP TIP
        // ===============================
        sf::CircleShape tip(W * 0.35f, 24);

        tip.setOrigin({ tip.getRadius(), tip.getRadius() });
        tip.setPosition({ pos.x, pos.y - rows * spacing - spacing * 0.2f });
        tip.setFillColor(lighten(light, 15));

        window.draw(tip);
    }
}