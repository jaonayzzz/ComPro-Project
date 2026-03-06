#include "ui_banner.h"
#include "config.h"
#include "font_manager.h"

#include <imgui.h>
#include <imgui-SFML.h>
#include <map>
#include <vector>
#include <string>
#include <iostream>

struct BannerData
{
    std::vector<std::string> images;
};

static std::map<AppState, BannerData> bannerMap;
static std::map<AppState, bool> stateEnabled;
static std::map<AppState, bool> hasShown;
static std::map<std::string, sf::Texture> textureCache;

static bool systemEnabled = true;
static bool isActive = false;
static bool isFadingOut = false;
static float fadeAlpha = 0.f;
static int currentIndex = 0;
static AppState activeState;
static bool popupOpened = false;

// ======================================================
// PUBLIC
// ======================================================
void setBannerSystemEnabled(bool enabled)
{
    systemEnabled = enabled;
}

void setBannerEnabledForState(AppState state, bool enabled)
{
    stateEnabled[state] = enabled;
}

void resetBannerShown(AppState state)
{
    hasShown[state] = false;
}

// ======================================================
void initBannerSystem()
{
    bannerMap[AppState::MAIN_MENU] =
        { { "../picture/banner_welcome.png" } };

    bannerMap[AppState::CUSTOM_MODE] =
        { { "../picture/banner_custom_1.png",
            "../picture/banner_custom_2.png" } };

    stateEnabled[AppState::MAIN_MENU]   = true;
    stateEnabled[AppState::CUSTOM_MODE] = false;
}

// ======================================================
static sf::Texture& getTexture(const std::string& path)
{
    if (textureCache.find(path) == textureCache.end())
    {
        sf::Texture tex;
        if (!tex.loadFromFile(path))
            std::cerr << "Failed to load: " << path << "\n";
        tex.setSmooth(true);
        textureCache[path] = tex;
    }
    return textureCache[path];
}

// ======================================================
// DRAW
// ======================================================
bool drawBanner(sf::RenderWindow& window, AppState currentState)
{
    if (!systemEnabled) return false;
    if (!stateEnabled[currentState]) return false;
    if (hasShown[currentState] && !isActive) return false;

    if (!isActive)
    {
        if (bannerMap.find(currentState) == bannerMap.end())
            return false;

        isActive = true;
        isFadingOut = false;
        fadeAlpha = 0.f;
        currentIndex = 0;
        activeState = currentState;
        popupOpened = false;
    }

    if (activeState != currentState)
    {
        isActive = false;
        return false;
    }

    // ================= Fade =================
    if (!isFadingOut)
    {
        fadeAlpha += 10.f;
        if (fadeAlpha > 255.f) fadeAlpha = 255.f;
    }
    else
    {
        fadeAlpha -= 15.f;
        if (fadeAlpha <= 0.f)
        {
            fadeAlpha = 0.f;
            isActive = false;
            hasShown[currentState] = true;
            popupOpened = false;
            return false;
        }
    }

    // ================= Overlay =================
    sf::RectangleShape overlay(
        sf::Vector2f(window.getSize()));
    overlay.setFillColor(sf::Color(
        0,0,0,
        static_cast<std::uint8_t>(
            170.f * (fadeAlpha/255.f))));
    window.draw(overlay);

    // ================= Banner Image =================
    auto& data = bannerMap[currentState];
    auto& tex = getTexture(data.images[currentIndex]);

    sf::Sprite sprite(tex);

    float scale =
        0.75f * window.getSize().x /
        tex.getSize().x;

    sprite.setScale(sf::Vector2f(scale, scale));

    sf::FloatRect bounds =
        sprite.getGlobalBounds();

    sprite.setPosition(sf::Vector2f(
        window.getSize().x / 2.f - bounds.size.x / 2.f,
        window.getSize().y / 2.f - bounds.size.y / 2.f));

    sprite.setColor(sf::Color(
        255,255,255,
        static_cast<std::uint8_t>(fadeAlpha)));

    window.draw(sprite);

    // ================= ImGui Modal =================
    if (!popupOpened)
    {
        ImGui::OpenPopup("BannerPopup");
        popupOpened = true;
    }

    ImGui::SetNextWindowSize(
        ImVec2((float)window.getSize().x,
               (float)window.getSize().y));

    if (ImGui::BeginPopupModal("BannerPopup",
        nullptr,
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBackground))
    {
        ImGui::PushFont(FONT_BODY);

        ImGui::SetCursorPos(
            ImVec2(window.getSize().x - 150.f, 40.f));

        if (ImGui::Button("Close", ImVec2(120,45)))
            isFadingOut = true;

        if (data.images.size() > 1)
        {
            ImGui::SetCursorPos(
                ImVec2(120.f,
                       window.getSize().y - 110.f));

            if (ImGui::Button("< Back", ImVec2(120,45)))
                if (currentIndex > 0)
                    currentIndex--;

            ImGui::SetCursorPos(
                ImVec2(window.getSize().x - 240.f,
                       window.getSize().y - 110.f));

            if (ImGui::Button("Next >", ImVec2(120,45)))
                if (currentIndex < data.images.size()-1)
                    currentIndex++;
        }

        ImGui::PopFont();
        ImGui::EndPopup();
    }

    return true;
}