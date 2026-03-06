#pragma once
#include <SFML/Graphics.hpp>
#include "app_state.h"

// ===== System =====
void initBannerSystem();
bool drawBanner(sf::RenderWindow& window, AppState currentState);

// ===== Control API =====
void setBannerSystemEnabled(bool enabled);
void setBannerEnabledForState(AppState state, bool enabled);
void resetBannerShown(AppState state);