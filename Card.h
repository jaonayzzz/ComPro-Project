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

void DrawOrderSystemUI(OrderCardData& cardData,bool &isCustommode,string &finalmsg,AppState& app) {
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
        if(!isCustommode){
            ImGui::InputTextMultiline("##Message", cardData.message, IM_ARRAYSIZE(cardData.message), 
                                  ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 4),ImGuiInputTextFlags_ReadOnly);
        }else{
            ImGui::InputTextMultiline("##Message", cardData.message, IM_ARRAYSIZE(cardData.message), 
                                  ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 4));
        }
        

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