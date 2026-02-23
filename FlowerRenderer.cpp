#include "FlowerRenderer.h"
#include <cmath>
#include <SFML/System/Angle.hpp>

constexpr float PI = 3.1415926f;

// ======================================================
// Utility – soften color
// ======================================================
sf::Color soften(sf::Color c, int delta)
{
    auto clamp = [](int v){ return std::max(0, std::min(255, v)); };
    return sf::Color(
        clamp(c.r + delta),
        clamp(c.g + delta),
        clamp(c.b + delta),
        c.a
    );
}

// ======================================================
// Dispatcher
// ======================================================
void FlowerRenderer::draw(
    sf::RenderWindow& window,
    FlowerType type,
    sf::Vector2f pos,
    float size,
    float rot
)
{
    switch (type)
    {
    // 🌹 Rose Family
    case FlowerType::RedRose:
        drawRose(window, pos, size, rot, sf::Color(178,34,34));
        break;
    case FlowerType::PinkRose:
        drawRose(window, pos, size, rot, sf::Color(255,105,180));
        break;
    case FlowerType::WhiteRose:
        drawRose(window, pos, size, rot, sf::Color(240,240,240));
        break;

    // 🌷 Tulip Family
    case FlowerType::YellowTulip:
        drawTulip(window, pos, size, rot, sf::Color(255,215,0));
        break;
    case FlowerType::RedTulip:
        drawTulip(window, pos, size, rot, sf::Color(220,20,60));
        break;
    case FlowerType::WhiteTulip:
        drawTulip(window, pos, size, rot, sf::Color(245,245,245));
        break;

    // 🪷 Lotus
    case FlowerType::Lotus:
        drawLotus(window, pos, size, rot);
        break;

    // 🌼 Daisy-style
    case FlowerType::WhiteDaisy:
    case FlowerType::YellowDaisy:
    case FlowerType::Chamomile:
    case FlowerType::WhiteAnemone:
    case FlowerType::RedAnemone:
    case FlowerType::Jasmine:
        drawDaisy(window, pos, size, rot,
                  sf::Color(255, 240, 220),
                  sf::Color(255, 210, 80));
        break;

    default:
        drawDaisy(window, pos, size, rot,
                  sf::Color(250, 230, 240),
                  sf::Color(255, 210, 100));
        break;
    }
}

//
// ======================================================
// 🌼 Daisy (Soft Layered)
// ======================================================
void FlowerRenderer::drawDaisy(
    sf::RenderWindow& window,
    sf::Vector2f pos,
    float size,
    float rot,
    sf::Color petal,
    sf::Color center
)
{
    float R = size * 0.5f;
    int petals = 14;

    for (int i = 0; i < petals; ++i)
    {
        float angle = i * 360.f / petals + rot;

        sf::CircleShape p(R * 0.35f, 40);
        p.setOrigin({R * 0.35f, R * 0.35f});
        p.setPosition(pos);
        p.setRotation(sf::degrees(angle));
        p.setScale({0.4f, 1.2f});
        p.setFillColor(petal);
        window.draw(p);
    }

    sf::CircleShape c(R * 0.35f, 40);
    c.setOrigin({R * 0.35f, R * 0.35f});
    c.setPosition(pos);
    c.setFillColor(center);
    window.draw(c);
}

//
// ======================================================
// 🌹 Rose (Layered Swirl)
// ======================================================
void FlowerRenderer::drawRose(
    sf::RenderWindow& window,
    sf::Vector2f pos,
    float size,
    float rot,
    sf::Color baseColor
)
{
    float R = size * 0.5f;

    for (int layer = 0; layer < 3; ++layer)
    {
        int petals = 6 + layer * 2;
        float scaleY = 0.6f + layer * 0.1f;

        for (int i = 0; i < petals; ++i)
        {
            float angle = i * 360.f / petals + rot + layer * 10.f;

            sf::CircleShape p(R * (0.45f - layer * 0.08f), 48);
            p.setOrigin({p.getRadius(), p.getRadius()});
            p.setPosition(pos);
            p.setRotation(sf::degrees(angle));
            p.setScale({0.8f, scaleY});
            p.setFillColor(soften(baseColor, layer * 15));
            window.draw(p);
        }
    }

    sf::CircleShape core(R * 0.25f, 48);
    core.setOrigin({R * 0.25f, R * 0.25f});
    core.setPosition(pos);
    core.setFillColor(soften(baseColor, 30));
    window.draw(core);
}

//
// ======================================================
// 🌷 Tulip (Cup Shape)
// ======================================================
void FlowerRenderer::drawTulip(
    sf::RenderWindow& window,
    sf::Vector2f pos,
    float size,
    float rot,
    sf::Color color
)
{
    float R = size * 0.5f;

    for (int i = -1; i <= 1; ++i)
    {
        sf::CircleShape petal(R * 0.6f, 48);
        petal.setOrigin({R * 0.6f, R * 0.6f});
        petal.setPosition({pos.x + i * R * 0.3f, pos.y});
        petal.setRotation(sf::degrees(rot + i * 8.f));
        petal.setScale({0.7f, 1.3f});
        petal.setFillColor(soften(color, i * 10));
        window.draw(petal);
    }

    sf::CircleShape base(R * 0.35f, 40);
    base.setOrigin({R * 0.35f, R * 0.35f});
    base.setPosition({pos.x, pos.y + R * 0.25f});
    base.setFillColor(soften(color, -20));
    window.draw(base);
}

//
// ======================================================
// 🪷 Lotus (Layered Symmetry)
// ======================================================
void FlowerRenderer::drawLotus(
    sf::RenderWindow& window,
    sf::Vector2f pos,
    float size,
    float rot
)
{
    float R = size * 0.5f;
    sf::Color base(255, 190, 210);

    for (int layer = 0; layer < 2; ++layer)
    {
        int petals = 8 + layer * 4;

        for (int i = 0; i < petals; ++i)
        {
            float angle = i * 360.f / petals + rot;

            sf::CircleShape p(R * (0.5f - layer * 0.1f), 48);
            p.setOrigin({p.getRadius(), p.getRadius()});
            p.setPosition(pos);
            p.setRotation(sf::degrees(angle));
            p.setScale({0.5f, 1.4f});
            p.setFillColor(soften(base, layer * 20));
            window.draw(p);
        }
    }

    sf::CircleShape center(R * 0.3f, 40);
    center.setOrigin({R * 0.3f, R * 0.3f});
    center.setPosition(pos);
    center.setFillColor(sf::Color(255, 220, 150));
    window.draw(center);
}