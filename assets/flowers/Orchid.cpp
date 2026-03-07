#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <algorithm>

namespace Orchid
{
    static sf::Color tint(sf::Color c, int d)
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
        float size
    )
    {
        const float R = size * 0.5f;

        sf::Color base(245, 230, 245);
        sf::Color pink(235, 170, 215);
        sf::Color purple(185, 90, 170);
        sf::Color coreColor(255, 200, 220);

        // ===============================
        // BACK PETALS (SIDE)
        // ===============================
        for (int side : {-1, 1})
        {
            sf::CircleShape petal(R * 0.45f, 64);

            petal.setOrigin({ petal.getRadius(), petal.getRadius() });
            petal.setPosition({
                pos.x + side * R * 0.55f,
                pos.y - R * 0.1f
            });
            petal.setScale({ 0.6f, 1.4f });
            petal.setRotation(sf::degrees(side * 15.f));
            petal.setFillColor(base);

            window.draw(petal);
        }

        // ===============================
        // TOP PETAL (DORSAL SEPAL)
        // ===============================
        {
            sf::CircleShape top(R * 0.50f, 64);

            top.setOrigin({ top.getRadius(), top.getRadius() });
            top.setPosition({ pos.x, pos.y - R * 0.65f });
            top.setScale({ 0.7f, 1.6f });
            top.setFillColor(tint(base, 5));

            window.draw(top);
        }

        // ===============================
        // FRONT PETALS
        // ===============================
        for (int side : {-1, 1})
        {
            sf::CircleShape petal(R * 0.38f, 64);

            petal.setOrigin({ petal.getRadius(), petal.getRadius() });
            petal.setPosition({
                pos.x + side * R * 0.35f,
                pos.y + R * 0.1f
            });
            petal.setScale({ 0.6f, 1.2f });
            petal.setRotation(sf::degrees(side * -12.f));
            petal.setFillColor(tint(base, -5));

            window.draw(petal);
        }

        // ===============================
        // LABELLUM (LIP)
        // ===============================
        sf::ConvexShape lip;
        lip.setPointCount(7);

        lip.setPoint(0, { 0.f, 0.f });
        lip.setPoint(1, {  R * 0.25f, R * 0.15f });
        lip.setPoint(2, {  R * 0.35f, R * 0.45f });
        lip.setPoint(3, {  0.f, R * 0.65f });
        lip.setPoint(4, { -R * 0.35f, R * 0.45f });
        lip.setPoint(5, { -R * 0.25f, R * 0.15f });
        lip.setPoint(6, {  0.f, R * 0.05f });

        lip.setPosition({ pos.x, pos.y + R * 0.15f });
        lip.setFillColor(pink);

        window.draw(lip);

        // ===============================
        // INNER LIP DETAIL
        // ===============================
        sf::ConvexShape inner;
        inner.setPointCount(5);

        inner.setPoint(0, { 0.f, R * 0.05f });
        inner.setPoint(1, {  R * 0.15f, R * 0.25f });
        inner.setPoint(2, {  0.f, R * 0.45f });
        inner.setPoint(3, { -R * 0.15f, R * 0.25f });
        inner.setPoint(4, { 0.f, R * 0.1f });

        inner.setPosition({ pos.x, pos.y + R * 0.18f });
        inner.setFillColor(purple);

        window.draw(inner);

        // ===============================
        // CORE
        // ===============================
        sf::CircleShape core(R * 0.10f, 32);

        core.setOrigin({ core.getRadius(), core.getRadius() });
        core.setPosition(pos);
        core.setFillColor(coreColor);

        window.draw(core);
    }
}