#pragma once

#include <cmath>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <algorithm>
#include <sstream>
#include <map>
#include "ui/Card.h"
#include "AppState.h"
#include "render/flowerRender.h"

using namespace ImGui;
using namespace std;

static int currentpages;
static bool showreceipt = false;
static float price = 0.f;
static float total = 0.f;

void printreceipt(const vector<Flower> &,const UserSelection &,const string &,float);

void ShowReceiptModal(bool* open, const vector<Flower>& items,OrderCardData &cardData,
    float& currentheight,const UserSelection &user,const vector<Container> &container,AppState &appstate,int &currentpages){
    if (*open) OpenPopup("ReceiptPopup");
    float targetheight = 450.0f;
    float printspeed = 150.0f; //ความเร็ว
    if(currentheight < targetheight){
        currentheight += printspeed*GetIO().DeltaTime; //get time/frame rate to create animation
        if(currentheight > targetheight){
            currentheight = targetheight;
        }
    }
    const ImGuiViewport* viewport = GetMainViewport();
    float marginTop = 100.0f; // ระยะห่างจากขอบบนหน้าจอเกมที่ต้องการ

    SetNextWindowPos(
    ImVec2(viewport->Pos.x + viewport->Size.x * 0.5f, viewport->Pos.y + marginTop), 
    ImGuiCond_Always, 
    ImVec2(0.5f, 0.0f) // ให้ "หัว" ของหน้าต่างทุกอันเริ่มที่จุด marginTop เดียวกัน
    );
    SetNextWindowSize(ImVec2(300.f, currentheight), ImGuiCond_Always);
    PushStyleVar(ImGuiStyleVar_WindowRounding, 10.f);
    PushStyleColor(ImGuiCol_PopupBg,      ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
    PushStyleColor(ImGuiCol_Text,         ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
    PushStyleColor(ImGuiCol_TitleBgActive,ImVec4(0.9f, 0.9f, 0.9f, 1.0f));
    PushStyleColor(ImGuiCol_TitleBg,      ImVec4(0.9f, 0.9f, 0.9f, 1.0f));

    if (BeginPopupModal("ReceiptPopup", nullptr, 
                                ImGuiWindowFlags_NoResize |
                                ImGuiWindowFlags_NoCollapse | 
                                ImGuiWindowFlags_NoTitleBar | 
                                ImGuiWindowFlags_NoMove |
                                ImGuiWindowFlags_NoScrollbar |         
                                ImGuiWindowFlags_NoScrollWithMouse)) {
        float windowWidth = GetWindowWidth();

        float tw1 = CalcTextSize("Receipt").x;
        SetCursorPosX((windowWidth - tw1) * 0.5f);
        Text("Receipt");

        float tw2 = CalcTextSize("Chonampay Florist").x;
        SetCursorPosX((windowWidth - tw2) * 0.5f);
        Text("Chonampay Florist");

        Separator();
        Spacing();

        total = 0.f;

        if(user.getReturnState() == AppState::PRESET_PAGE){ //if from preset
            Text("%-20s",user.presetName.c_str());
            SameLine(220);
            total += user.totalAmount;
            Text("%.2f",total);
        }else{ //from others
            //count each type of flower
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
                Text("%-20s", name.c_str());
                SameLine(170);
                Text("x %d", qty);
                SameLine(220);
                Text("%.2f",subtotal);
                total += subtotal;
            }
            if (!user.containerType.empty()) {
                Text("%-20s", user.containerType.c_str());
                Text("Size: %s", user.containerSize.c_str());
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

        Spacing();
        Separator();
        Spacing();

        if (cardData.message[0] != '\0') {
            Text("Card Message:");
            TextWrapped("%s", cardData.message);
            Spacing();
            Separator();
            Spacing();
        }

        Text("TOTAL");
        SameLine(220);
        Text("%.2f", total);

        Spacing();
        Separator();
        Spacing();

        float btnWidth = 120.f;
        PushFont(FONT_BODY);
        SetCursorPosX((windowWidth - btnWidth) * 0.5f);
        if (Button("Close Receipt", ImVec2(btnWidth, 30))) {
            CloseCurrentPopup();
            *open = false;
            appstate = AppState::MAIN_MENU;
            //clear data
            cardData.Clear();
            nameBuf[0] = '\0';
            currentpages = 0;
        }
        SetCursorPosX((windowWidth - btnWidth) * 0.5f);
        if(Button("Print Receipt",ImVec2(btnWidth,30))){
            printreceipt(items,user,cardData.message,total);
        }
        PopFont();

        PopStyleColor(4);
        PopStyleVar(1);
        EndPopup();
    } else {
        PopStyleColor(4);
        PopStyleVar(1); 
    }
}

void showcard(OrderCardData cardData) {
    // โหลดรูปหน้าการ์ดและหลังการ์ด
    static sf::Texture cardBgFront; 
    static sf::Texture cardBgBack; 
    static bool isTextureLoaded = false;
    if (!isTextureLoaded) {
        if (!cardBgBack.loadFromFile("../assets/picture/card.png")) {
            cout << "Failed to load card.png!" << endl;
        }
        if (!cardBgFront.loadFromFile("../assets/picture/card_back.png")) {
            cout << "Failed to load card_back.png!" << endl;
        }
        isTextureLoaded = true;
        cout << "picture loaded successful!" << endl;
    }

    //flip card part
    static float flipTimer = 0.0f;
    static bool isFlipped = false;   
    static bool isAnimating = false; 
    float flipSpeed = 3.0f;          

    if (isAnimating) {
        flipTimer += GetIO().DeltaTime * flipSpeed;
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
    
    //use cos to flip
    float scaleX = abs(cos(isAnimating ? flipTimer : 0.0f)); 
    float currentWidth = baseWidth * scaleX; 

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize   |
                             ImGuiWindowFlags_NoCollapse |
                             ImGuiWindowFlags_NoTitleBar |
                             ImGuiWindowFlags_NoScrollbar |
                             ImGuiWindowFlags_NoTitleBar |
                             ImGuiWindowFlags_NoBackground |
                             ImGuiWindowFlags_NoMove |
                             ImGuiWindowFlags_NoScrollWithMouse;

    BeginChild("CardPanel", ImVec2(600.0,-100.0f), false, flags);
    
    ImVec2 panelSize = GetWindowSize();
    
    // จัดตำแหน่งกึ่งกลางเสมอ (แม้การ์ดจะหดเล็กลง)
    float posX = (panelSize.x - currentWidth) * 0.5f;
    float posY = ((panelSize.y - cardHeight) * 0.5f) - 10.0f;

    SetCursorPos(ImVec2(posX, posY));

    bool showBackSide = isFlipped;
    if (isAnimating) {
        // ถ้าอยู่ในช่วงแอนิเมชันเกินครึ่งทาง (Pi/2) ให้สลับการแสดงผลล่วงหน้า
        if (flipTimer > 1.5707f) { 
            showBackSide = !isFlipped; 
        }
    }

    // สร้างปุ่มแบบมองไม่เห็นครอบการ์ดไว้ (กดรูปเพื่อพลิก)
    PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
    PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);

    PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
    PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
    PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));

    //draw picture
    if (showBackSide) {
        if (ImageButton("BackCard", cardBgBack, sf::Vector2f(currentWidth, cardHeight))) {
            if (!isAnimating) isAnimating = true; // กดเพื่อพลิกกลับ
        }
    } else {
        if (ImageButton("FrontCard", cardBgFront, sf::Vector2f(currentWidth, cardHeight))) {
            if (!isAnimating) isAnimating = true; // กดเพื่อพลิกไปด้านหลัง
        }
    }

    PopStyleColor(4);
    PopStyleVar(2);

    //place message
    PushFont(FONT_BODY);
    if (showBackSide && !isAnimating) {
        SetCursorPos(ImVec2(posX + 100, posY + 150));
        Text("To : %s", cardData.recipient);

    float paddingX = 50.0f;
    
    // ความกว้างสูงสุดที่ข้อความจะแสดงได้ก่อนถูกปัดบรรทัด
    float wrapWidth = currentWidth- (paddingX * 2.0f); 

    // คำนวณขนาดของก้อนข้อความ "หลังจาก" ถูกตัดบรรทัดแล้ว 
    ImVec2 textSize = CalcTextSize(cardData.message, NULL, false, wrapWidth);

    SetCursorPos(ImVec2(
        posX + paddingX, 
        posY + (cardHeight - textSize.y) * 0.5f
    ));

    //การตัดบรรทัดข้อความ
    PushTextWrapPos(GetCursorPosX() + wrapWidth);
    
    Text("%s", cardData.message);
    
    PopTextWrapPos();

        ImVec2 senderSize = CalcTextSize(cardData.sender);
        SetCursorPos(ImVec2(
            posX + currentWidth - senderSize.x - 190, 
            posY + cardHeight - senderSize.y - 90
        ));
        Text("From : %s", cardData.sender);
    }
    PopFont();

    EndChild();
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

        map<string, int> flowerCount;
        map<string, float> flowerPrice;

        for (const auto& flower : flowers) {
            flowerCount[flower.name]++;
            flowerPrice[flower.name] = flower.price;
        }

        for(const auto& pair : flowerCount){
            string name = pair.first;
            int qty = pair.second;
            float price = flowerPrice[name];
            float subtotal = price * qty; 
            
            dest << left << setw(20) << name          
                 << "x " << left << setw(5) << qty    
                 << right << setw(10) << fixed << setprecision(2) << subtotal << "\n"; 
        }
        
        dest << user.containerType << endl;
        dest << "Size : " << user.containerSize << endl;
        dest << "----------------------------------------\n";
        
        if(!cardmessage.empty()){
            dest << "Card Message:\n";
            dest << cardmessage << endl;
            dest << "----------------------------------------\n";
        }

        dest << left << setw(30) << "TOTAL"
                << right << setw(10) << fixed << setprecision(2) << total << "\n";
        dest << "----------------------------------------\n";

        dest.close(); 
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
    
    float originalWidth = preset.getLocalBounds().size.x;
    float originalHeight = preset.getLocalBounds().size.y;

    
    float scaleX = maxTargetwidth / originalWidth;
    float scaleY = maxTargetheight / originalHeight;

    
    float finalScale = min(scaleX, scaleY);

    preset.setScale(sf::Vector2f(finalScale, finalScale));
    float paddingright = 50.0f;
    float paddingtop = 50.0f;
    float imagewidth = preset.getGlobalBounds().size.x;
    float posx = WINDOW_WIDTH - imagewidth - paddingright;
    float posy = paddingtop;
    preset.setPosition(sf::Vector2f(posx,posy));
    window.draw(preset);
}

void confirm(const vector<Flower>& items, const string& cardmessage,
    sf::RenderWindow& window, const UserSelection& selection,const vector<sf::Texture>& flowerTextures,
    const OrderCardData carddata,const vector<Container>& container,AppState &appstate){
    ImVec2 center = GetMainViewport()->GetCenter();
    SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.75f));
    SetNextWindowSize(ImVec2(480.f, 460.f), ImGuiCond_Always);

    static float receiptheight = 0.0f; //initial height of receipt

    ImVec2 screenSize = GetIO().DisplaySize; 

    SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always); 
    SetNextWindowSize(screenSize, ImGuiCond_Always);
        
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize   |
                             ImGuiWindowFlags_NoCollapse |
                             ImGuiWindowFlags_NoTitleBar |
                             ImGuiWindowFlags_NoScrollbar |
                             ImGuiWindowFlags_NoTitleBar |
                             ImGuiWindowFlags_NoBackground |
                             ImGuiWindowFlags_NoMove |
                             ImGuiWindowFlags_NoScrollWithMouse;
    
    Begin("Confirm page", nullptr, flags);
    
    if(currentpages == 0){
        if(cardData.haveCard){ //card == yes
            showcard(cardData);
            PushFont(FONT_BODY);
            float textwidth = CalcTextSize("Click to flip the card!").x;
            SetCursorPos(ImVec2((screenSize.x-textwidth)*0.5,600.0f));
            Text("Click the card to flip it!");
            PopFont();
        }else{//card == no
            static sf::Texture emptyCardTexture;
            static bool isTextureLoad = false;

            if (!isTextureLoad) {
                if (emptyCardTexture.loadFromFile("../assets/picture/card_no.png")) {
                    emptyCardTexture.setSmooth(true); 
                } else {
                    cout << "Error: can't load card_no.png \n";
                }
                isTextureLoad = true; 
            }

            float emptyCardWidth = 400.0f;
            float emptyCardHeight = 600.0f;
            
            float cardPosX = (screenSize.x * 0.25f) - (emptyCardWidth * 0.5f); 
            float cardPosY = 50.0f;
            SetCursorPos(ImVec2(cardPosX, cardPosY));
            Image(emptyCardTexture, ImVec2(emptyCardWidth, emptyCardHeight));
        }
        if(selection.getReturnState() == AppState::PRESET_PAGE){
            printpreset(window,selection);
        }else{
            renderBouquet(window,selection,flowerTextures);
        }

        float buttonWidth = 150.0f;
        float buttonHeight = 40.0f;

        SetCursorPos(ImVec2((screenSize.x - buttonWidth) * 0.5f , 650.0f));

        PushFont(FONT_BODY);
        PushStyleColor(ImGuiCol_Button, COLOR_BUTTON);         
        PushStyleColor(ImGuiCol_ButtonHovered, COLOR_BUTTON_HOVER);
        PushStyleColor(ImGuiCol_ButtonActive, COLOR_BUTTON_ACTIVE);   
        if (Button("Check out", ImVec2(buttonWidth, buttonHeight))) {
            cout << "กำลังดำเนินการชำระเงิน...\n";
            currentpages = 1;
        }
        PopStyleColor(3);
        PopFont();
        

    }else if(currentpages == 1){
        showreceipt = true;
        ShowReceiptModal(&showreceipt,items,cardData,receiptheight,selection,container,appstate,currentpages);
    }
    End();
}