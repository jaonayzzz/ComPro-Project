#pragma once

#include "card.h"
#include "app_state.h"
#include <cmath>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <algorithm>
#include <sstream>
#include <map>

using namespace ImGui;
using namespace std;

static int currentpages;
static bool showreceipt = false;
static float price = 0.f;
static float total = 0.f;

void printreceipt(const vector<Flower> &,const UserSelection &,const string &,float);

void ShowReceiptModal(bool* open, const std::vector<Flower>& items, const std::string& cardmessage,
    float& currentheight,const UserSelection &user,const vector<Container> &container,AppState &appstate){
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

        total = 0.f;

        if(user.getReturnState() == AppState::PRESET_PAGE){
            Text("%-20s",user.presetName.c_str());
            SameLine(220);
            total += user.totalAmount;
            Text("%.2f",total);
        }else{
            map<string,int> itemcount;
            map<string,float> itemprice;
            for(const auto& item:items){
                itemcount[item.name]++;
                itemprice[item.name] = static_cast<float>(item.price);
            }
            for (const auto& pair : itemcount) {
                string name = pair.first;
                int qty = pair.second;
                float price = itemprice[name];
                float subtotal = price*qty;
                ImGui::Text("%-20s", name.c_str());
                ImGui::SameLine(170);
                ImGui::Text("x %d", qty);
                SameLine(220);
                Text("%.2f",subtotal);
                total += subtotal;
            }
            if (!user.containerType.empty()) {
                ImGui::Text("%-20s", user.containerType.c_str());
                ImGui::Text("Size: %s", user.containerSize.c_str());
                SameLine(220);
                for(int i=0;i<sizeof(containerList);i++){
                    if(containerList[i].type == user.containerType && containerList[i].size == user.containerSize){
                        price = containerList[i].basePrice;
                        break;
                    }
                }
                Text("%.2f",price);
                total += price;
            }
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
        PushFont(FONT_BODY);
        ImGui::SetCursorPosX((windowWidth - btnWidth) * 0.5f);
        if (ImGui::Button("Close Receipt", ImVec2(btnWidth, 30))) {
            ImGui::CloseCurrentPopup();
            *open = false;
            CloseCurrentPopup();
            appstate = AppState::MAIN_MENU;
        }
        ImGui::SetCursorPosX((windowWidth - btnWidth) * 0.5f);
        if(Button("Print Receipt",ImVec2(btnWidth,30))){
            printreceipt(items,user,cardmessage,total);
        }
        PopFont();

        ImGui::PopStyleColor(4);
        ImGui::PopStyleVar(1);
        ImGui::EndPopup();
    } else {
        ImGui::PopStyleColor(4);
        ImGui::PopStyleVar(1); 
    }
}

void showcard(OrderCardData cardData) {
    // โหลดรูปหน้าการ์ดและหลังการ์ด
    static sf::Texture cardBgFront; 
    static sf::Texture cardBgBack; // ต้องมีรูปปกหลังการ์ดด้วย
    static bool isTextureLoaded = false;

    if (!isTextureLoaded) {
        if (!cardBgBack.loadFromFile("../picture/card.png")) {
            cout << "Failed to load card.png!" << endl;
        }
        if (!cardBgFront.loadFromFile("../picture/card_back.png")) {
            cout << "Failed to load card_back.png!" << std::endl;
        }
        isTextureLoaded = true;
        cout << "picture loaded successful!" << endl;
    }

    // ----------------------------------------------------
    // ตัวแปรควบคุมแอนิเมชันการพลิกการ์ด
    // ----------------------------------------------------
    static float flipTimer = 0.0f;
    static bool isFlipped = false;   // ตอนนี้อยู่ด้านหลังหรือเปล่า?
    static bool isAnimating = false; // กำลังเล่นแอนิเมชันอยู่ไหม?
    float flipSpeed = 3.0f;          // ความเร็วในการพลิก (ยิ่งค่าน้อย ยิ่งช้า)

    // อัปเดตแอนิเมชันทุกเฟรม
    if (isAnimating) {
        flipTimer += ImGui::GetIO().DeltaTime * flipSpeed;
        if (flipTimer >= 3.14159f) { // ใช้ค่า Pi (ครบรอบการพลิก 1 ครั้ง)
            flipTimer = 0.0f;
            isAnimating = false;
            isFlipped = !isFlipped;  // สลับสถานะหน้า-หลังเมื่อพลิกเสร็จ
        }
    }

    // ----------------------------------------------------
    // คำนวณความกว้างที่เปลี่ยนไปตามแอนิเมชัน
    // ----------------------------------------------------
    float baseWidth = 400.0f;
    float cardHeight = 600.0f;
    
    // ใช้ฟังก์ชัน Cosine สร้างความสมูท: 
    // ค่า scaleX จะค่อยๆ ลดจาก 1.0 -> 0.0 -> 1.0
    float scaleX = std::abs(std::cos(isAnimating ? flipTimer : 0.0f)); 
    float currentWidth = baseWidth * scaleX; 

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize   |
                             ImGuiWindowFlags_NoCollapse |
                             ImGuiWindowFlags_NoTitleBar |
                             ImGuiWindowFlags_NoScrollbar |
                             ImGuiWindowFlags_NoTitleBar |
                             ImGuiWindowFlags_NoBackground |
                             ImGuiWindowFlags_NoMove |
                             ImGuiWindowFlags_NoScrollWithMouse;

    ImGui::BeginChild("CardPanel", ImVec2(600.0,-100.0f), false, flags);
    
    ImVec2 panelSize = ImGui::GetWindowSize();
    
    // จัดตำแหน่งกึ่งกลางเสมอ (แม้การ์ดจะหดเล็กลง)
    float posX = (panelSize.x - currentWidth) * 0.5f;
    float posY = ((panelSize.y - cardHeight) * 0.5f) - 10.0f;

    ImGui::SetCursorPos(ImVec2(posX, posY));

    // ----------------------------------------------------
    // ตัดสินใจว่าจะโชว์รูปหน้าไหน (ถ้าพลิกเกินครึ่งทางแล้วให้เปลี่ยนรูป)
    // ----------------------------------------------------
    bool showBackSide = isFlipped;
    if (isAnimating) {
        // ถ้าอยู่ในช่วงแอนิเมชันเกินครึ่งทาง (Pi/2) ให้สลับการแสดงผลล่วงหน้า
        if (flipTimer > 1.5707f) { 
            showBackSide = !isFlipped; 
        }
    }

    // สร้างปุ่มแบบมองไม่เห็นครอบการ์ดไว้ (กดรูปเพื่อพลิก)
    // 1. ตั้งค่าระยะห่าง (Padding) และความหนาของเส้นขอบให้เป็น 0
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);

    // 2. ตั้งค่าสีของปุ่มและสีของเส้นขอบให้โปร่งใส (Alpha = 0)
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));

    // 3. วาดปุ่มรูปภาพ
    if (showBackSide) {
        if (ImGui::ImageButton("BackCard", cardBgBack, sf::Vector2f(currentWidth, cardHeight))) {
            if (!isAnimating) isAnimating = true; // กดเพื่อพลิกกลับ
        }
    } else {
        if (ImGui::ImageButton("FrontCard", cardBgFront, sf::Vector2f(currentWidth, cardHeight))) {
            if (!isAnimating) isAnimating = true; // กดเพื่อพลิกไปด้านหลัง
        }
    }

    // 4. คืนค่าสไตล์และสีกลับเป็นปกติ (มี PopStyleColor 4 ตัว และ PopStyleVar 2 ตัว)
    ImGui::PopStyleColor(4);
    ImGui::PopStyleVar(2);

    // ----------------------------------------------------
    // วาดข้อความ (เฉพาะตอนที่อยู่ด้านหน้าและไม่ได้พลิกอยู่)
    // ----------------------------------------------------
    PushFont(FONT_BODY);
    if (showBackSide && !isAnimating) {
        ImGui::SetCursorPos(ImVec2(posX + 100, posY + 150));
        ImGui::Text("To : %s", cardData.recipient);

    float paddingX = 50.0f;
    
    // ความกว้างสูงสุดที่ข้อความจะแสดงได้ก่อนถูกปัดบรรทัด
    float wrapWidth = currentWidth- (paddingX * 2.0f); 

    // คำนวณขนาดของก้อนข้อความ "หลังจาก" ถูกตัดบรรทัดแล้ว (สังเกตพารามิเตอร์ตัวสุดท้าย)
    ImVec2 textSize = ImGui::CalcTextSize(cardData.message, NULL, false, wrapWidth);

    // เซ็ตตำแหน่งให้ก้อนข้อความอยู่กึ่งกลางการ์ดพอดี
    ImGui::SetCursorPos(ImVec2(
        posX + paddingX, 
        posY + (cardHeight - textSize.y) * 0.5f
    ));

    // สั่งให้ ImGui เริ่มระบบตัดบรรทัด (Wrap) โดยอิงจากความกว้างที่ตั้งไว้
    ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + wrapWidth);
    
    // วาดข้อความลงไป
    ImGui::Text("%s", cardData.message);
    
    // ปิดระบบตัดบรรทัด (สำคัญมาก ต้องมีปิดท้ายเสมอ)
    ImGui::PopTextWrapPos();

        ImVec2 senderSize = ImGui::CalcTextSize(cardData.sender);
        ImGui::SetCursorPos(ImVec2(
            posX + currentWidth - senderSize.x - 190, 
            posY + cardHeight - senderSize.y - 90
        ));
        ImGui::Text("From : %s", cardData.sender);
    }
    PopFont();

    ImGui::EndChild();
}

string filenames(){
        auto now = chrono::system_clock::now();
        time_t now_c = chrono::system_clock::to_time_t(now);
        stringstream ss;
        ss << "../Receipt/receipt_" << put_time(localtime(&now_c),"%Y%m%d_%H%M%S") << ".txt";
        return ss.str();
    }

void printreceipt(const vector<Flower> &flowers,const UserSelection &user,const string &cardmessage,float total){
    string filename = filenames(); 
    ofstream dest(filename);
    if(dest.is_open()){
        dest << "                Receipt        \n";
        dest << "           Chonampay Florist   \n";
        dest << "----------------------------------------\n";

        std::map<std::string, int> flowerCount;
        std::map<std::string, float> flowerPrice;

        for (const auto& flower : flowers) {
            flowerCount[flower.name]++;
            flowerPrice[flower.name] = flower.price;
        }

        // 2. ลูปแสดงผลจาก Map ที่จับกลุ่มแล้ว
        for(const auto& pair : flowerCount){
            std::string name = pair.first;
            int qty = pair.second;
            float price = flowerPrice[name];
            float subtotal = price * qty; // ราคารวมต่อรายการ
            
            dest << std::left << std::setw(20) << name          // คอลัมน์ชื่อดอกไม้
                 << "x " << std::left << std::setw(5) << qty    // คอลัมน์จำนวน
                 << std::right << std::setw(10) << std::fixed << std::setprecision(2) << subtotal << "\n"; // คอลัมน์ราคารวม
        }
        
        dest << user.containerType << endl;
        dest << "Size : " << user.containerSize << endl;
        dest << "----------------------------------------\n";
        
        if(!cardmessage.empty()){
            dest << "Card Message:\n";
            dest << cardmessage << endl;
            dest << "----------------------------------------\n";
        }

        dest << std::left << std::setw(30) << "TOTAL"
                << std::right << std::setw(10) << std::fixed << std::setprecision(2) << total << "\n";
        dest << "----------------------------------------\n";

        dest.close(); // ปิดไฟล์เมื่อเขียนเสร็จ
        cout << "print receipt  " << filename << " successfully !\n";
        string command;
        #ifdef _WIN32
            command = "notepad " + filename;
        #elif __APPLE__
            command = "open " + filename;
        #endif
        system(command.c_str());
    } else {
        cout << "can't not open file\n";
    }
}

void printpreset(sf::RenderWindow &window,const UserSelection &selection){
    static sf::Texture flowerpreset;
    static string loadedimagepath = "";
    if(loadedimagepath != selection.presetImagePath){
        if(!flowerpreset.loadFromFile(selection.presetImagePath)){
            cout << "load flowerpreset unsuccessfully" << endl;
        }else{
            loadedimagepath = selection.presetImagePath;
        }
    }
    sf::Sprite preset(flowerpreset);
    float maxTargetwidth =480.0f;
    float maxTargetheight = 600.0f;
    // ดึงขนาดดั้งเดิมของรูปภาพ
    float originalWidth = preset.getLocalBounds().size.x;
    float originalHeight = preset.getLocalBounds().size.y;

    // คำนวณอัตราส่วนที่ต้องย่อลง
    float scaleX = maxTargetwidth / originalWidth;
    float scaleY = maxTargetheight / originalHeight;

    // เลือกอัตราส่วนที่ "น้อยกว่า" เพื่อให้รูปพอดีกรอบและสัดส่วนไม่เพี้ยน (Keep Aspect Ratio)
    float finalScale = std::min(scaleX, scaleY);

    // สั่งย่อรูปภาพ (ส่งค่า x และ y เท่ากันเพื่อคงสัดส่วนเดิมไว้)
    preset.setScale(sf::Vector2f(finalScale, finalScale));
    float paddingright = 50.0f;
    float paddingtop = 50.0f;
    float imagewidth = preset.getGlobalBounds().size.x;
    float posx = WINDOW_WIDTH - imagewidth - paddingright;
    float posy = paddingtop;
    preset.setPosition(sf::Vector2f(posx,posy));
    window.draw(preset);
}