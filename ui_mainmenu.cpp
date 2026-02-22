#include "ui_mainmenu.h"

#include <imgui.h>
#include <vector>
#include <string>

void drawMainMenu(AppState& state, UserSelection& selection)
{
    static int occasionIndex = 0;
    static int containerIndex = 0;
    static int budget = 500;

    const char* occasionItems[] = {
        "Condolences",
        "Congratulations",
        "Love & Care"
    };

    ImGui::Begin("Flower Shop - Main Menu");

    ImGui::Text("Welcome to Flower Shop Game");
    ImGui::Separator();

    ImGui::Text("Select Occasion");
    ImGui::Combo("##Occasion", &occasionIndex, occasionItems, IM_ARRAYSIZE(occasionItems));

    ImGui::Spacing();
    ImGui::Text("Select Container");

    std::vector<std::string> containerLabels;
    for (auto& c : containerList) {
        containerLabels.push_back(c.type + " - Size " + c.size);
    }

    std::vector<const char*> containerCStr;
    for (auto& s : containerLabels) {
        containerCStr.push_back(s.c_str());
    }

    ImGui::Combo(
        "##Container",
        &containerIndex,
        containerCStr.data(),
        static_cast<int>(containerCStr.size())
    );

    const Container& selectedContainer = containerList[containerIndex];

    ImGui::Text(
        "Flowers: %d - %d | Base Price: %d",
        selectedContainer.minF,
        selectedContainer.maxF,
        selectedContainer.basePrice
    );

    ImGui::Text(
        "Estimated Price Range: %d - %d",
        selectedContainer.minTotal,
        selectedContainer.maxTotal
    );

    ImGui::Spacing();
    ImGui::Text("Set Budget");
    ImGui::InputInt("##Budget", &budget);

    if (budget < selectedContainer.minTotal) {
        ImGui::TextColored(
            ImVec4(1.f, 0.f, 0.f, 1.f),
            "Warning: Budget is lower than minimum possible price!"
        );
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    if (ImGui::Button("Custom Mode", ImVec2(150, 40))) {
        selection.occasion = occasionItems[occasionIndex];
        selection.containerType = selectedContainer.type;
        selection.containerSize = selectedContainer.size;
        selection.budget = budget;
        selection.isCustomMode = true;

        state = AppState::CUSTOM_MODE;
    }

    ImGui::SameLine();

    if (ImGui::Button("Random Mode", ImVec2(150, 40))) {
        selection.occasion = occasionItems[occasionIndex];
        selection.containerType = selectedContainer.type;
        selection.containerSize = selectedContainer.size;
        selection.budget = budget;
        selection.isCustomMode = false;

        state = AppState::RANDOM_MODE;
    }

    ImGui::Spacing();
    ImGui::Separator();

    if (ImGui::Button("Exit")) {
        state = AppState::EXIT;
    }
//
    ImGui::End();
}