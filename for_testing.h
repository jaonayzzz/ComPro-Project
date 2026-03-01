#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/System/Clock.hpp>
#include <iostream>
#include <optional>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

#include "config.h"
#include "font_manager.h"
#include "ui_mainmenu.h"
#include "data_models.h"
#include "FlowerRenderer.h"

inline void SendYourFeelings() {
    float textWidth = ImGui::CalcTextSize("Send Your Feelings").x;
    float centerX = (WINDOW_WIDTH*0.9f - textWidth*1.0f) * 0.5f; 
    ImGui::SetCursorPosX(centerX); 
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.1f, 0.1f, 0.44f, 1.0f)); 
    ImGui::SetWindowFontScale(1.0f); 
    ImGui::Text("Send Your Feelings");
    ImGui::SetWindowFontScale(1.0f); 
    ImGui::PopStyleColor(1); 
}
inline void OrderSummary() {
    float textWidth = ImGui::CalcTextSize("Order Summary").x;
    float centerX = (WINDOW_WIDTH*0.9f - textWidth*1.0f) * 0.5f; 
    ImGui::SetCursorPosX(centerX); 
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.1f, 0.1f, 0.44f, 1.0f)); 
    ImGui::SetWindowFontScale(1.0f); 
    ImGui::Text("Order Summary");
    ImGui::SetWindowFontScale(1.0f); 
    ImGui::PopStyleColor(1); 
}