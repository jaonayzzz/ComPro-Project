#pragma once
#include "data_models.h"
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <iostream>
#include <optional>
#include "config.h"
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <string>
#include "font_manager.h"
#include "app_state.h"

using namespace std;
using namespace ImGui;


OrderCardData cardData;

static int         currentpage  = 0;
static std::string message      = "";
static string finalmessage = "";
static bool        show_receipt = false;
static int         randindex    = 0;
static char nameBuf[128] = "";
static bool isCustommode = false;
static bool havecard = false;

struct cards {
    string occasion, greeting;
};

vector<cards> blessing = {
    {"Condolences",    "With heartfelt sympathy on the passing of [Name]."},
    {"Condolences",    "May memories of [Name] bring you peace and comfort."},
    {"Condolences",    "Gone from our sight, but never from our hearts."},
    {"Condolences",    "Sending you love and strength in this time of loss."},
    {"Condolences",    "I am so sorry for your loss. I'm thinking of you."},
    {"Congratulations","Congratulations! This is a well-deserved success."},
    {"Congratulations","So thrilled for you! Enjoy every moment of this achievement."},
    {"Congratulations","Hats off to you! You did an amazing job."},
    {"Congratulations","Cheers to your next chapter! Best of luck."},
    {"Congratulations","You nailed it! So proud of you."},
    {"Love & Care",    "You mean the world to me."},
    {"Love & Care",    "Just wanted to send you some love and positive vibes."},
    {"Love & Care",    "No matter what happens, I'm always here for you."},
    {"Love & Care",    "Take good care of yourself. I miss you!"},
    {"Love & Care",    "Sending you a big hug to brighten your day."},
};

//logic random
static string getRandomMessage(string occasions, int random) {
    vector<string> u = {};
    for (int i = 0; i < (int)blessing.size(); i++) {
        if (occasions == blessing[i].occasion)
            u.push_back(blessing[i].greeting);
    }
    if (random >= (int)u.size()) return "";
    return u[random];
}

//draw ui random
static void showRandomCard(string& message,string &finalmsg, int random) {
    float windowWidth = ImGui::GetWindowWidth(); 

    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Text,    ImVec4(0.0f, 0.0f, 0.0f, 1.0f));

    size_t pos = message.find("[Name]");
    if (pos != string::npos) {
        string before = message.substr(0, pos);
        string after  = message.substr(pos + 6);

        float beforew  = ImGui::CalcTextSize(before.c_str()).x;
        float afterw   = ImGui::CalcTextSize(after.c_str()).x;
        float spacing  = ImGui::GetStyle().ItemSpacing.x;
        float inputw   = 120.f;
        float totalw   = beforew + spacing + inputw + spacing + afterw;

        ImGui::SetCursorPosX((windowWidth - totalw) / 2.f);
        ImGui::TextUnformatted(before.c_str());
        ImGui::SameLine();

        ImGui::PushItemWidth(inputw);
        string inputLabel = "##nameInput" + to_string(random);
        InputText(inputLabel.c_str(),nameBuf,sizeof(nameBuf));
        ImGui::PopItemWidth();

        ImGui::SameLine();
        ImGui::TextUnformatted(after.c_str());

        finalmsg = before + string(nameBuf) + after;
    } else {
        float textwidth = ImGui::CalcTextSize(message.c_str()).x;
        ImGui::SetCursorPosX((windowWidth - textwidth) * 0.5f);
        ImGui::TextUnformatted(message.c_str());
        finalmsg = message;
    }

    ImGui::PopStyleColor(2); 
}


/*static void showCustomCard(string& message) {
    ImGui::PushStyleColor(ImGuiCol_FrameBg,        ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.9f, 0.9f, 0.9f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive,  ImVec4(0.8f, 0.8f, 0.8f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Text,           ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.f);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,  ImVec2(10.f, 8.f));

    static char buf[1024] = "";
    ImGui::InputTextMultiline("##card", buf, sizeof(buf), ImVec2(300, 150));
    message = string(buf); // sync กับ message

    ImGui::PopStyleColor(4);
    ImGui::PopStyleVar(2);
}*/


/*void ShowReceiptModal(bool* open, const std::vector<Flower>& items, const std::string& cardmessage,float& currentheight) {
    if (*open) ImGui::OpenPopup("ReceiptPopup");
    float targetheight = 450.0f;
    float printspeed = 150.0f; //ความเร็ว
    if(currentheight < targetheight){
        currentheight += printspeed*ImGui::GetIO().DeltaTime;
        if(currentheight > targetheight){
            currentheight = targetheight;
        }
    }
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    float marginTop = 100.0f; // ระยะห่างจากขอบบนหน้าจอเกมที่ต้องการ

    ImGui::SetNextWindowPos(
    ImVec2(viewport->Pos.x + viewport->Size.x * 0.5f, viewport->Pos.y + marginTop), 
    ImGuiCond_Always, 
    ImVec2(0.5f, 0.0f) // ให้ "หัว" ของหน้าต่างทุกอันเริ่มที่จุด marginTop เดียวกัน
    );
    ImGui::SetNextWindowSize(ImVec2(300.f, currentheight), ImGuiCond_Always);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.f);
    ImGui::PushStyleColor(ImGuiCol_PopupBg,      ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Text,         ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive,ImVec4(0.9f, 0.9f, 0.9f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_TitleBg,      ImVec4(0.9f, 0.9f, 0.9f, 1.0f));

    if (ImGui::BeginPopupModal("ReceiptPopup", nullptr, 
                                ImGuiWindowFlags_NoResize |
                                ImGuiWindowFlags_NoCollapse | 
                                ImGuiWindowFlags_NoTitleBar | 
                                ImGuiWindowFlags_NoMove |
                                ImGuiWindowFlags_NoScrollbar |         
                                ImGuiWindowFlags_NoScrollWithMouse)) {
        float windowWidth = ImGui::GetWindowWidth();

        float tw1 = ImGui::CalcTextSize("Receipt").x;
        ImGui::SetCursorPosX((windowWidth - tw1) * 0.5f);
        ImGui::Text("Receipt");

        float tw2 = ImGui::CalcTextSize("Chonampay Florist").x;
        ImGui::SetCursorPosX((windowWidth - tw2) * 0.5f);
        ImGui::Text("Chonampay Florist");

        ImGui::Separator();
        ImGui::Spacing();

        float total = 0.f;
        for (const auto& item : items) {
            ImGui::Text("%-20s", item.name.c_str());
            ImGui::SameLine(220);
            ImGui::Text("%.2f", static_cast<float>(item.price));
            total += item.price;
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        if (!cardmessage.empty()) {
            ImGui::Text("Card Message:");
            ImGui::TextWrapped("%s", cardmessage.c_str());
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
        }

        ImGui::Text("TOTAL");
        ImGui::SameLine(220);
        ImGui::Text("%.2f", total);

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        float btnWidth = 120.f;
        ImGui::SetCursorPosX((windowWidth - btnWidth) * 0.5f);
        if (ImGui::Button("Close Receipt", ImVec2(btnWidth, 30))) {
            ImGui::CloseCurrentPopup();
            *open = false;
        }

        ImGui::PopStyleColor(4);
        ImGui::PopStyleVar(1);
        ImGui::EndPopup();
    } else {
        ImGui::PopStyleColor(4);
        ImGui::PopStyleVar(1); 
    }
}*/

void DrawOrderSystemUI(OrderCardData& cardData,bool &isCustommode,string &finalmsg,AppState& app,int&currentpage) {
    // กำหนดขนาดหน้าต่างหลัก
    ImVec2 center = GetMainViewport()->GetCenter();
    SetNextWindowPos(center,ImGuiCond_Appearing,ImVec2(0.5f,0.5f));
    ImGui::SetNextWindowSize(ImVec2(800, 500), ImGuiCond_FirstUseEver);
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize   |
                             ImGuiWindowFlags_NoCollapse |
                             ImGuiWindowFlags_NoTitleBar |
                             ImGuiWindowFlags_NoMove;
    ImGui::Begin("Flower Shop - Custom Order & Card System",nullptr,flags);

    float windowHeight = ImGui::GetWindowHeight();
    float contentHeight = 420.0f; // อิงจากความสูงของการ์ด (cardSize.y) ที่เราตั้งไว้
    ImGui::SetCursorPosY((windowHeight - contentHeight) / 2.0f);

    // สร้างตาราง 2 คอลัมน์ (ซ้าย: กรอกข้อมูล, ขวา: พรีวิวการ์ด)
    if (ImGui::BeginTable("OrderTable", 2, ImGuiTableFlags_BordersInnerV)) {
        
        // ==========================================
        // คอลัมน์ที่ 1: ส่วนฟอร์มกรอกข้อมูลออเดอร์ (Input)
        // ==========================================
        PushFont(FONT_BODY);
        ImGui::TableNextColumn();
        ImVec2 formSize(300.0f, 420.0f); // ล็อคความกว้างฟอร์มให้เท่ากับความกว้างการ์ด
        float leftColumnWidth = ImGui::GetColumnWidth();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (leftColumnWidth - formSize.x) / 2.0f);

        BeginChild("FormArea", formSize, false, ImGuiWindowFlags_NoScrollbar);

        ImGui::Text("Card Details");
        ImGui::Separator();
        ImGui::Spacing();

        // ช่องกรอกชื่อผู้รับ (To)
        ImGui::Text("Recipient (To):");
        // IM_ARRAYSIZE เป็น Macro ของ ImGui ที่ช่วยหาขนาดของ char array อัตโนมัติ

        ImGui::InputText("##To", cardData.recipient, IM_ARRAYSIZE(cardData.recipient)); 
        
        ImGui::Spacing();

        // ช่องกรอกชื่อผู้ส่ง (From)
        ImGui::Text("Sender (From):");
        ImGui::InputText("##From", cardData.sender, IM_ARRAYSIZE(cardData.sender));
        
        ImGui::Spacing();

        // ช่องกรอกข้อความอวยพร (Message) แบบหลายบรรทัด
        ImGui::Text("Greeting Message:");
        // ImVec2(-FLT_MIN, ...) หมายถึงให้ความกว้างขยายเต็มพื้นที่ที่เหลือ ส่วนความสูงกำหนดไว้ที่ประมาณ 4 บรรทัด
        ImGui::InputTextMultiline("##Message", cardData.message, IM_ARRAYSIZE(cardData.message), 
                                  ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 4));
        
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        // ปุ่มยืนยันออเดอร์และพิมพ์ใบเสร็จ
        ImGui::PushStyleColor(ImGuiCol_Button, COLOR_BUTTON); // เปลี่ยนปุ่มเป็นสีเขียว
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, COLOR_BUTTON_HOVER);
        if (ImGui::Button("Confirm Order & Generate Receipt", ImVec2(-FLT_MIN, 40))) {
            app = AppState::CONFIRM;
            cout << "Order Confirmed for: "<< cardData.recipient << endl;
            cout << "Generating Receipt...\n";
            finalmsg = string(cardData.message);
            show_receipt = true;
            currentpage = 0;
        }
        if(Button("Back",ImVec2(-FLT_MIN,40))){
            currentpage = 1;
        }
        ImGui::PopStyleColor(2); // คืนค่าสีปุ่ม
        PopFont();
        EndChild();


        // ==========================================
        // คอลัมน์ที่ 2: ส่วนแสดงผลการ์ดพรีวิว (Live Preview)
        // ==========================================
        ImGui::TableNextColumn();
        
        // จัดการ์ดให้อยู่กึ่งกลางของคอลัมน์ขวา
        ImVec2 cardSize(300, 420);
        float columnWidth = ImGui::GetColumnWidth();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (columnWidth - cardSize.x) / 2.0f);
        
        // เริ่มวาดการ์ด (นำเทคนิคตกแต่งจากโค้ดที่แล้วมาใช้)
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 12.0f);
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(1.0f, 0.98f, 0.94f, 1.0f)); 
        PushFont(FONT_BODY);
        
        // ใส่ ID ให้ Child Window
        ImGui::BeginChild("CardPreview", cardSize, true, ImGuiWindowFlags_NoScrollbar);

        // 1. รูปภาพหน้าการ์ด (อิงจากชนิดดอกไม้ที่ลูกค้าเลือก)
        ImVec2 imageSize(260, 180); 
        ImGui::SetCursorPosX((ImGui::GetWindowWidth() - imageSize.x) / 2.0f);
        ImGui::SetCursorPosY(15.0f);
        //ImGui::Image(cardData.coverImage, imageSize);
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        // 2. ผู้รับ (To)
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
        ImGui::Text("To: %s", cardData.recipient); // ดึงค่าจาก char array มาแสดงสดๆ
        ImGui::Spacing();

        // 3. ข้อความ (Message)
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.7f, 0.3f, 0.4f, 1.0f));
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.0f);
        ImGui::TextWrapped("%s", cardData.message);
        ImGui::PopStyleColor();
        ImGui::Spacing();

        // 4. ผู้ส่ง (From)
        std::string fromText = "From: " + std::string(cardData.sender);
        float textWidth = ImGui::CalcTextSize(fromText.c_str()).x;
        ImGui::SetCursorPosX(ImGui::GetWindowWidth() - textWidth - 15.0f); 
        ImGui::SetCursorPosY(cardSize.y - 40.0f); 
        ImGui::Text("%s", fromText.c_str());
        
        ImGui::PopStyleColor(); // จบส่วนสีตัวอักษร
        
        ImGui::EndChild();
        ImGui::PopStyleColor(); // จบสีพื้นหลังการ์ด
        ImGui::PopStyleVar();   // จบการทำให้ขอบมน
        PopFont();

        ImGui::EndTable();
    }

    ImGui::End();
}

void card(UserSelection &user, AppState& appstate){
    //,sf::RenderWindow &window,UserSelection &selection,vector<sf::Texture> &flowerTextures) {

    float windowWidth    = 480.f;
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
    if(currentpage != 5){
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
            cardData.haveCard = true;
            currentpage = 1; //go to random or custom
        }
        ImGui::SameLine(0.f, 60.f);
        if (ImGui::Button("No", ImVec2(80, 30))) {
            cardData.haveCard = false;
            appstate = AppState::CONFIRM; //ต้องแก้เซม go to print receipt
        }
        SetCursorPos(ImVec2((windowWidth - 80.f)*0.5f,290.f));
        if(Button("Back",ImVec2(80,30))){
            appstate = AppState::SUMMARY; 
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
            message     = getRandomMessage(user.occasion, randindex);
            isCustommode = false;
            currentpage = 3; //โชว์ข้อความแรนด้อม
        }

        ImGui::SetCursorPos(ImVec2(190.f, 260.f));
        if (ImGui::Button("Custom", ImVec2(100, 30))) {
            memset(cardData.message,0,sizeof(cardData.message));
            memset(cardData.recipient, 0, sizeof(cardData.recipient));
            memset(cardData.sender, 0, sizeof(cardData.sender));
            isCustommode = true;
            currentpage = 5; //go to func custom
        }

        ImGui::SetCursorPos(ImVec2((windowWidth - 80.f) * 0.5f, 310.f));
        if (ImGui::Button("< Back", ImVec2(80, 30))) {
            cardData.haveCard = false;
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
        showRandomCard(message,finalmessage,randindex);

        float buttonwidth = 100.f;
        float buttonheight  = 30.f;
        float spacing = GetStyle().ItemSpacing.x;
        float totalwidth   = (buttonwidth * 3) + (spacing * 2);
        ImGui::SetCursorPos(ImVec2((windowWidth - totalwidth) * 0.5f, 380.f));

        if (ImGui::Button("< Back", ImVec2(buttonwidth, buttonheight))) {
            message     = "";
            randindex   = 0;
            currentpage = 1;
        }
        SameLine(0.f,spacing);
        if(Button("Re-Random",ImVec2(buttonwidth,buttonheight))){
            memset(nameBuf,0,sizeof(nameBuf));
            randindex   = rand()%5;
            message     = getRandomMessage(user.occasion, randindex);
            isCustommode = false;
        }
        ImGui::SameLine(0.f, spacing);
        if (ImGui::Button("confirm >", ImVec2(buttonwidth, buttonheight))) {
            strncpy(cardData.message,finalmessage.c_str(),sizeof(cardData.message));
            cardData.message[sizeof(cardData.message)-1] = '\0';
            currentpage = 5;
            //receiptheight = 0.0f;
        }
        ImGui::PopFont();
    }

    // ---- PAGE 4: Custom Card ----
    /*else if (currentpage == 4) {
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
            currentpage = 5;
        }

        ImGui::PopFont();
    }*/
    
    // ---- PAGE 2: Receipt ----
    /*else if (currentpage == 2) {
        show_receipt = true;
        ShowReceiptModal(&show_receipt, u, message,receiptheight);
    }*/
    ImGui::End();
    } 
    
    if(currentpage == 5){
        //renderBouquet(window,selection,flowerTextures);
        DrawOrderSystemUI(cardData,isCustommode,finalmessage,appstate,currentpage);
    }
}