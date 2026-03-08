#pragma once
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <iostream>
#include <optional>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <string>
#include "Config.h"
#include "DataModels.h"
#include "Core/FontManager.h"
#include "AppState.h"

using namespace std;
using namespace ImGui;


OrderCardData cardData;

static int         currentpage  = 0;
static string message      = "";
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
    float windowWidth = GetWindowWidth(); 

    PushStyleColor(ImGuiCol_FrameBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); //white
    PushStyleColor(ImGuiCol_Text,    ImVec4(0.0f, 0.0f, 0.0f, 1.0f)); //black

    size_t pos = message.find("[Name]");
    if (pos != string::npos) {
        string before = message.substr(0, pos);
        string after  = message.substr(pos + 6);

        float beforew  = CalcTextSize(before.c_str()).x;
        float afterw   = CalcTextSize(after.c_str()).x;
        float spacing  = GetStyle().ItemSpacing.x;
        float inputw   = 120.f;
        float totalw   = beforew + spacing + inputw + spacing + afterw;

        SetCursorPosX((windowWidth - totalw) / 2.f);
        TextUnformatted(before.c_str());
        SameLine();

        PushItemWidth(inputw);
        string inputLabel = "##nameInput" + to_string(random);
        InputText(inputLabel.c_str(),nameBuf,sizeof(nameBuf));
        PopItemWidth();

        SameLine();
        TextUnformatted(after.c_str());

        finalmsg = before + string(nameBuf) + after;
    } else {
        float textwidth = CalcTextSize(message.c_str()).x;
        SetCursorPosX((windowWidth - textwidth) * 0.5f);
        TextUnformatted(message.c_str());
        finalmsg = message;
    }

    PopStyleColor(2); 
}

void DrawOrderSystemUI(OrderCardData& cardData,bool &isCustommode,string &finalmsg,AppState& app,int&currentpage) {
    //fix window size
    ImVec2 center = GetMainViewport()->GetCenter();
    SetNextWindowPos(center,ImGuiCond_Appearing,ImVec2(0.5f,0.5f));
    SetNextWindowSize(ImVec2(800, 500), ImGuiCond_FirstUseEver);
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize   |
                             ImGuiWindowFlags_NoCollapse |
                             ImGuiWindowFlags_NoTitleBar |
                             ImGuiWindowFlags_NoMove;
    Begin("Flower Shop - Custom Order & Card System",nullptr,flags);

    float windowHeight = GetWindowHeight();
    float contentHeight = 420.0f; //ref from card height
    SetCursorPosY((windowHeight - contentHeight) / 2.0f);

    //create table with 2 column
    if (BeginTable("OrderTable", 2, ImGuiTableFlags_BordersInnerV)) {
        
        //input part
        PushFont(FONT_BODY);
        TableNextColumn();
        ImVec2 formSize(300.0f, 420.0f); //fix card size
        float leftColumnWidth = GetColumnWidth();
        SetCursorPosX(GetCursorPosX() + (leftColumnWidth - formSize.x) / 2.0f);
        //create window ย่อย
        BeginChild("FormArea", formSize, false, ImGuiWindowFlags_NoScrollbar);

        Text("Card Details");
        Separator();
        Spacing();

        // ช่องกรอกชื่อผู้รับ (To)
        Text("Recipient (To):");
        // IM_ARRAYSIZE เป็น Macro ของ ImGui ที่ช่วยหาขนาดของ char array อัตโนมัติ
        InputText("##To", cardData.recipient, IM_ARRAYSIZE(cardData.recipient)); 
        
        Spacing();

        // ช่องกรอกชื่อผู้ส่ง (From)
        Text("Sender (From):");
        InputText("##From", cardData.sender, IM_ARRAYSIZE(cardData.sender));
        
        Spacing();

        // ช่องกรอกข้อความอวยพร (Message) แบบหลายบรรทัด
        Text("Greeting Message:");
        InputTextMultiline("##Message", cardData.message, IM_ARRAYSIZE(cardData.message), 
                                  ImVec2(-FLT_MIN, GetTextLineHeight() * 4));
        
        Spacing();
        Spacing();
        Separator();
        Spacing();

        // ปุ่มยืนยันออเดอร์และพิมพ์ใบเสร็จ
        PushStyleColor(ImGuiCol_Button, COLOR_BUTTON); 
        PushStyleColor(ImGuiCol_ButtonHovered, COLOR_BUTTON_HOVER);
        if (Button("Confirm Order & Generate Receipt", ImVec2(-FLT_MIN, 40))) {
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
        PopStyleColor(2); // คืนค่าสีปุ่ม
        PopFont();
        EndChild();


        //card preview
        TableNextColumn();
        
        // จัดการ์ดให้อยู่กึ่งกลางของคอลัมน์ขวา
        ImVec2 cardSize(300, 420);
        float columnWidth = GetColumnWidth();
        SetCursorPosX(GetCursorPosX() + (columnWidth - cardSize.x) / 2.0f);
        
        // เริ่มวาดการ์ด (นำเทคนิคตกแต่งจากโค้ดที่แล้วมาใช้)
        PushStyleVar(ImGuiStyleVar_ChildRounding, 12.0f);
        PushStyleColor(ImGuiCol_ChildBg, ImVec4(1.0f, 0.98f, 0.94f, 1.0f)); //off-white
        PushFont(FONT_BODY);
        
        // ใส่ ID ให้ Child Window
        BeginChild("CardPreview", cardSize, true, ImGuiWindowFlags_NoScrollbar);

        // 2. ผู้รับ (To)
        PushStyleColor(ImGuiCol_Text, ImVec4(0.2f, 0.2f, 0.2f, 1.0f)); //dark gray
        Text("To: %s", cardData.recipient); 
        Spacing();

        // 3. ข้อความ (Message)
        PushStyleColor(ImGuiCol_Text, ImVec4(0.7f, 0.3f, 0.4f, 1.0f)); //rose red
        SetCursorPosY(GetCursorPosY() + 10.0f);
        TextWrapped("%s", cardData.message);
        PopStyleColor();
        Spacing();

        // 4. ผู้ส่ง (From)
        string fromText = "From: " + string(cardData.sender);
        float textWidth = CalcTextSize(fromText.c_str()).x;
        SetCursorPosX(GetWindowWidth() - textWidth - 15.0f); 
        SetCursorPosY(cardSize.y - 40.0f); 
        Text("%s", fromText.c_str());
        
        PopStyleColor(); 
        
        EndChild();
        PopStyleColor(); 
        PopStyleVar();   
        PopFont();

        EndTable();
    }

    End();
}

void card(UserSelection &user, AppState& appstate){
    float windowWidth    = 480.f;
    ImVec2 center = GetMainViewport()->GetCenter();
    SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.75f));
    SetNextWindowSize(ImVec2(windowWidth, 460.f), ImGuiCond_Always);

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize   |
                             ImGuiWindowFlags_NoCollapse |
                             ImGuiWindowFlags_NoTitleBar |
                             ImGuiWindowFlags_NoMove;
    const ImGuiViewport* viewport = GetMainViewport();
    float marginTop = 80.0f;
    if(currentpage == 0 || currentpage == 1 || currentpage == 2){
        SetNextWindowPos(center,ImGuiCond_Always,ImVec2(0.5f,0.5f));
        SetNextWindowSize(ImVec2(480.f,460.f),ImGuiCond_Always);
    }
    if(currentpage != 5){
    Begin("Card Menu", nullptr, flags);
    
    // ---- PAGE 0: Would you like some card? ----
    if (currentpage == 0) {
        PushFont(FONT_TITLE);
        float textWidth = CalcTextSize("Would you like some card?").x;
        SetCursorPos(ImVec2((windowWidth - textWidth) / 2.f, 190.f));
        Text("Would you like some card?");
        PopFont();

        PushFont(FONT_BODY);
        PushStyleColor(ImGuiCol_Button,
            ImVec4(245/255.f, 139/255.f, 194/255.f, 0.8f));
        PushStyleColor(ImGuiCol_ButtonHovered,
            ImVec4(255/255.f, 160/255.f, 210/255.f, 1.0f));

        float totalbuttonwth = 80.f + 60.f + 80.f;
        SetCursorPos(ImVec2((windowWidth - totalbuttonwth) * 0.5f, 240.f));

        if (Button("Yes", ImVec2(80, 30))) {
            cardData.haveCard = true;
            currentpage = 1; //go to random or custom
        }
        SameLine(0.f, 60.f);
        if (Button("No", ImVec2(80, 30))) {
            cardData.haveCard = false;
            appstate = AppState::CONFIRM; //go to confirm
        }
        SetCursorPos(ImVec2((windowWidth - 80.f)*0.5f,290.f));
        if(Button("Back",ImVec2(80,30))){
            appstate = AppState::SUMMARY; 
        }

        PopStyleColor(2);
        PopFont();
    }

    // ---- PAGE 1: Random or Custom ----
    else if (currentpage == 1) {
        PushFont(FONT_TITLE);
        float textWidth = CalcTextSize("Random or Custom").x;
        SetCursorPos(ImVec2((windowWidth - textWidth) / 2.f, 180.f));
        Text("Random or Custom");
        PopFont();

        PushFont(FONT_BODY);

        SetCursorPos(ImVec2(190.f, 220.f));
        if (Button("Random", ImVec2(100, 30))) {
            randindex = rand()%5;
            message     = getRandomMessage(user.occasion, randindex);
            isCustommode = false;
            currentpage = 2; //โชว์ข้อความแรนด้อม
        }

        SetCursorPos(ImVec2(190.f, 260.f));
        if (Button("Custom", ImVec2(100, 30))) {
            memset(cardData.message,0,sizeof(cardData.message));
            memset(cardData.recipient, 0, sizeof(cardData.recipient));
            memset(cardData.sender, 0, sizeof(cardData.sender));
            isCustommode = true;
            currentpage = 5; //go to func custom
        }

        SetCursorPos(ImVec2((windowWidth - 80.f) * 0.5f, 310.f));
        if (Button("< Back", ImVec2(80, 30))) {
            cardData.haveCard = false;
            currentpage = 0; 
        }
        PopFont();
    }

    // ---- PAGE 2: Show Random Card Result ----
    else if (currentpage == 2) {
        PushFont(FONT_TITLE);
        float textwid = CalcTextSize("Your card result here!").x;
        SetCursorPos(ImVec2((windowWidth - textwid) / 2.f, 180.f));
        Text("Your card result here!");
        PopFont();

        PushFont(FONT_BODY);
        SetCursorPos(ImVec2(40.f, 220.f));

        showRandomCard(message,finalmessage,randindex);

        float buttonwidth = 100.f;
        float buttonheight  = 30.f;
        float spacing = GetStyle().ItemSpacing.x;
        float totalwidth   = (buttonwidth * 3) + (spacing * 2);
        SetCursorPos(ImVec2((windowWidth - totalwidth) * 0.5f, 380.f));

        if (Button("< Back", ImVec2(buttonwidth, buttonheight))) {
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
        SameLine(0.f, spacing);
        if (Button("confirm >", ImVec2(buttonwidth, buttonheight))) {
            strncpy(cardData.message,finalmessage.c_str(),sizeof(cardData.message));
            cardData.message[sizeof(cardData.message)-1] = '\0';
            currentpage = 5;
        }
        PopFont();
    }
    End();
    } 
    if(currentpage == 5){
        DrawOrderSystemUI(cardData,isCustommode,finalmessage,appstate,currentpage);
    }
}