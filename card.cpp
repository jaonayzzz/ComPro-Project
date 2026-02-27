#include "Card.h"
#include "app_state.h"

using namespace ImGui;

void card(const std::vector<Flower>& u, AppState& appstate, std::string occassions) {

    float windowWidth    = 480.f;
    static float receiptheight = 0.0f; //initial height of receipt
    

    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.75f));
    ImGui::SetNextWindowSize(ImVec2(windowWidth, 460.f), ImGuiCond_Always);

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize   |
                             ImGuiWindowFlags_NoCollapse |
                             ImGuiWindowFlags_NoTitleBar |
                             ImGuiWindowFlags_NoMove;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    float marginTop = 80.0f;
    if(currentpage == 0 || currentpage == 1 || currentpage == 3 || currentpage == 4){
        ImGui::SetNextWindowPos(center,ImGuiCond_Always,ImVec2(0.5f,0.5f));
        ImGui::SetNextWindowSize(ImVec2(480.f,460.f),ImGuiCond_Always);
    }
    if(currentpage == 2){
        float receiptmargintop = 100.0f;
        ImGui::SetNextWindowPos(
            ImVec2(viewport->Pos.x + viewport->Size.x * 0.5f, viewport->Pos.y + receiptmargintop), 
            ImGuiCond_Always, 
            ImVec2(0.5f, 0.0f)
        );
        ImGui::SetNextWindowSize(ImVec2(300.f, 450.f), ImGuiCond_Always);
    }
    ImGui::Begin("Card Menu", nullptr, flags);
    
    // ---- PAGE 0: Would you like some card? ----
    if (currentpage == 0) {
        ImGui::PushFont(FONT_TITLE);
        float textWidth = ImGui::CalcTextSize("Would you like some card?").x;
        ImGui::SetCursorPos(ImVec2((windowWidth - textWidth) / 2.f, 190.f));
        ImGui::Text("Would you like some card?");
        ImGui::PopFont();

        ImGui::PushFont(FONT_BODY);
        ImGui::PushStyleColor(ImGuiCol_Button,
            ImVec4(245/255.f, 139/255.f, 194/255.f, 0.8f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
            ImVec4(255/255.f, 160/255.f, 210/255.f, 1.0f));

        float totalbuttonwth = 80.f + 60.f + 80.f;
        ImGui::SetCursorPos(ImVec2((windowWidth - totalbuttonwth) * 0.5f, 240.f));

        if (ImGui::Button("Yes", ImVec2(80, 30))) {
            currentpage = 1; //go to random or custom
        }
        ImGui::SameLine(0.f, 60.f);
        if (ImGui::Button("No", ImVec2(80, 30))) {
            currentpage = 2; //go to print receipt
        }

        ImGui::PopStyleColor(2);
        ImGui::PopFont();
    }

    // ---- PAGE 1: Random or Custom ----
    else if (currentpage == 1) {
        ImGui::PushFont(FONT_TITLE);
        float textWidth = ImGui::CalcTextSize("Random or Custom").x;
        ImGui::SetCursorPos(ImVec2((windowWidth - textWidth) / 2.f, 180.f));
        ImGui::Text("Random or Custom");
        ImGui::PopFont();

        ImGui::PushFont(FONT_BODY);

        ImGui::SetCursorPos(ImVec2(190.f, 220.f));
        if (ImGui::Button("Random", ImVec2(100, 30))) {
            // ✅ ใช้ getRandomMessage() แทน randomCard()
            // set message ครั้งเดียวตอนกดปุ่ม ไม่ใช่ทุก frame
            randindex = rand()%5;
            message     = getRandomMessage(occassions, randindex);
            currentpage = 3; //โชว์ข้อความแรนด้อม
        }

        ImGui::SetCursorPos(ImVec2(190.f, 260.f));
        if (ImGui::Button("Custom", ImVec2(100, 30))) {
            currentpage = 4; //go to func custom
        }

        ImGui::SetCursorPos(ImVec2((windowWidth - 80.f) * 0.5f, 310.f));
        if (ImGui::Button("< Back", ImVec2(80, 30))) {
            currentpage = 0; //ask want card **may be change to summary dee kuaw**
        }

        ImGui::PopFont();
    }

    // ---- PAGE 3: Show Random Card Result ----
    else if (currentpage == 3) {
        ImGui::PushFont(FONT_TITLE);
        float textwid = ImGui::CalcTextSize("Your card result here!").x;
        ImGui::SetCursorPos(ImVec2((windowWidth - textwid) / 2.f, 180.f));
        ImGui::Text("Your card result here!");
        ImGui::PopFont();

        ImGui::PushFont(FONT_BODY);
        ImGui::SetCursorPos(ImVec2(40.f, 220.f));

        // ✅ ใช้ showRandomCard() แทน randomCard()
        // วาด UI ทุก frame — message ถูก set ไปแล้วตอนกดปุ่ม Random
        showRandomCard(message, randindex);

        float betweenwidth = 80.f;
        float gap          = 40.f;
        float totalwidth   = (betweenwidth * 2) + gap;
        ImGui::SetCursorPos(ImVec2((windowWidth - totalwidth) * 0.5f, 380.f));

        if (ImGui::Button("< Back", ImVec2(betweenwidth, 30))) {
            message     = "";
            randindex   = 0;
            currentpage = 1;
        }
        SameLine(0.f,gap);
        if(Button("Re-Random",ImVec2(betweenwidth,30))){
            message     = "";
            randindex   = 0;
            showRandomCard(message,randindex);
        }
        ImGui::SameLine(0.f, gap);
        if (ImGui::Button("confirm >", ImVec2(betweenwidth, 30))) {
            currentpage = 2;
            //receiptheight = 0.0f;
        }
        ImGui::PopFont();
    }

    // ---- PAGE 4: Custom Card ----
    else if (currentpage == 4) {
        ImGui::PushFont(FONT_TITLE);
        float textwid = ImGui::CalcTextSize("Please write your card here!").x;
        ImGui::SetCursorPos(ImVec2((windowWidth - textwid) / 2.f, 50.f));
        ImGui::Text("Please write your card here!");
        ImGui::PopFont();

        ImGui::PushFont(FONT_BODY);
        ImGui::SetCursorPos(ImVec2(90.f, 185.f));

        // ✅ ใช้ showCustomCard() แทน customCard()
        // รับ message& — sync ข้อความที่พิมพ์อัตโนมัติทุก frame
        showCustomCard(message);

        ImGui::SetCursorPos(ImVec2(200.f, 380.f));
        if (ImGui::Button("< Back", ImVec2(80, 30))) {
            currentpage = 1;
        }
        ImGui::SameLine(0.f, 120.f);
        if (ImGui::Button("confirm >", ImVec2(80, 30))) {
            currentpage = 2;
        }

        ImGui::PopFont();
    }

    // ---- PAGE 2: Receipt ----
    else if (currentpage == 2) {
        show_receipt = true;
        ShowReceiptModal(&show_receipt, u, message,receiptheight);
    }
    ImGui::End(); 
}