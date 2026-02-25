#include "ui_start.h"
#include "config.h"

#include <SFML/Graphics.hpp>
#include <SFML/System/Angle.hpp>
#include <filesystem>
#include <optional>
#include <cmath>
#include <iostream>

// =====================================================
// Resources
// =====================================================
static sf::Texture bgTexture;
static sf::Texture logoTexture;
static sf::Texture startBtnTexture;

static std::optional<sf::Sprite> bgSprite;
static std::optional<sf::Sprite> logoSprite;
static std::optional<sf::Sprite> startBtnSprite;

// =====================================================
// State
// =====================================================
static float swingTime = 0.f;
static float logoY     = -20.f;   // ปรับระดับโลโก้
static bool  pullingUp = false;
static bool  initialized = false;

// =====================================================
// Init
// =====================================================
void initStartScreen()
{
    if (initialized) return;

    // ---------- Background ----------
    if (!bgTexture.loadFromFile("../picture/Background.png"))
        std::cerr << "Failed to load Background.png\n";

    bgSprite.emplace(bgTexture);
    bgSprite->setScale(sf::Vector2f(
        (float)WINDOW_WIDTH  / bgTexture.getSize().x,
        (float)WINDOW_HEIGHT / bgTexture.getSize().y
    ));

    // ---------- Logo ----------
    if (!logoTexture.loadFromFile("../picture/Logo.png"))
        std::cerr << "Failed to load Logo.png\n";

    logoSprite.emplace(logoTexture);

    // Origin ด้านบนเหมือนป้ายห้อย
    logoSprite->setOrigin(sf::Vector2f(
        logoTexture.getSize().x / 2.f,
        0.f
    ));

    logoSprite->setPosition(sf::Vector2f(
        WINDOW_WIDTH / 2.f,
        logoY
    ));

    // ---------- Start Button (Image) ----------
    if (!startBtnTexture.loadFromFile("../picture/Start-Button.png"))
        std::cerr << "Failed to load Start-Button.png\n";

    startBtnSprite.emplace(startBtnTexture);

    // ตั้ง origin กลางภาพ
    startBtnSprite->setOrigin(sf::Vector2f(
        startBtnTexture.getSize().x / 2.f,
        startBtnTexture.getSize().y / 2.f
    ));

    // วางตำแหน่งให้ต่ำลง (ปรับได้)
    startBtnSprite->setPosition(sf::Vector2f(
        WINDOW_WIDTH / 2.f,
        WINDOW_HEIGHT - 100.f
    ));

    initialized = true;
}

// =====================================================
// Draw
// =====================================================
void drawStartScreen(sf::RenderWindow& window, AppState& state)
{
    if (!initialized) return;

    swingTime += 0.03f;

    // ---------- Background ----------
    if (bgSprite)
        window.draw(*bgSprite);

    // ---------- Logo Animation ----------
    if (logoSprite)
    {
        if (!pullingUp)
        {
            float deg = std::sin(swingTime) * 3.f;
            logoSprite->setRotation(sf::degrees(deg));
        }
        else
        {
            logoY -= 6.f;
            logoSprite->setPosition(sf::Vector2f(
                WINDOW_WIDTH / 2.f,
                logoY
            ));

            if (logoY < -400.f)
                state = AppState::MAIN_MENU;
        }

        window.draw(*logoSprite);
    }

    // ---------- Start Button ----------
    if (startBtnSprite)
    {
        sf::Vector2i mouse = sf::Mouse::getPosition(window);
        sf::Vector2f mouseF((float)mouse.x, (float)mouse.y);

        bool hovered = startBtnSprite->getGlobalBounds().contains(mouseF);

        // Hover effect เบา ๆ
        if (hovered)
            startBtnSprite->setScale(sf::Vector2f(1.05f, 1.05f));
        else
            startBtnSprite->setScale(sf::Vector2f(1.f, 1.f));

        window.draw(*startBtnSprite);

        // Click
        if (hovered &&
            sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            pullingUp = true;
        }
    }
}