#include "ui_loading.h"
#include "config.h"
#include "app_ready.h"

#include <SFML/Graphics.hpp>
#include <optional>
#include <iostream>

// =====================================================
// Resources (เบา)
// =====================================================
static sf::Texture bgTexture;
static sf::Texture logoTexture;

// =====================================================
// Sprites
// =====================================================
static std::optional<sf::Sprite> bgSprite;
static std::optional<sf::Sprite> logoSprite;

// =====================================================
// State
// =====================================================
static bool  initialized = false;
static float logoY = -400.f;
static bool  startDrop = false;

// =====================================================
// INIT
// =====================================================
void initLoadingScreen()
{
    if (initialized) return;

    if (!bgTexture.loadFromFile("../picture/Background.png"))
        std::cerr << "Failed to load Background.png\n";

    bgSprite.emplace(bgTexture);
    bgSprite->setScale(sf::Vector2f(
        (float)WINDOW_WIDTH  / bgTexture.getSize().x,
        (float)WINDOW_HEIGHT / bgTexture.getSize().y
    ));

    if (!logoTexture.loadFromFile("../picture/Logo.png"))
        std::cerr << "Failed to load Logo.png\n";

    logoSprite.emplace(logoTexture);
    logoSprite->setOrigin(sf::Vector2f(
        logoTexture.getSize().x / 2.f,
        0.f
    ));

    logoSprite->setPosition(sf::Vector2f(
        WINDOW_WIDTH / 2.f,
        logoY
    ));

    initialized = true;
}

// =====================================================
// UPDATE (ตรรกะล้วน)
// =====================================================
void updateLoadingScreen(AppState& state)
{
    updateAppReady();

    // ถ้าระบบพร้อม → เริ่ม animation ปิด
    if (appReady)
        startDrop = true;

    if (startDrop)
    {
        logoY += 6.f;
        if (logoY >= 80.f)
        {
            logoY = 80.f;
            state = AppState::START_SCREEN;
        }
    }
}

// =====================================================
// DRAW (ภาพล้วน)
// =====================================================
void drawLoadingScreen(sf::RenderWindow& window)
{
    if (!initialized) return;

    if (bgSprite)
        window.draw(*bgSprite);

    if (logoSprite)
    {
        logoSprite->setPosition(sf::Vector2f(
            WINDOW_WIDTH / 2.f,
            logoY
        ));
        window.draw(*logoSprite);
    }
}