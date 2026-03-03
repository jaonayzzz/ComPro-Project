#include "confirm.h"
#include "flowerRender.h"


using namespace ImGui;
using namespace std;

void confirm(const vector<Flower>& items, const string& cardmessage,
    sf::RenderWindow& window, const UserSelection& selection,const vector<sf::Texture>& flowerTextures,
    const OrderCardData carddata,const vector<Container>& container,AppState &appstate){
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.75f));
    ImGui::SetNextWindowSize(ImVec2(480.f, 460.f), ImGuiCond_Always);

    static float receiptheight = 0.0f; //initial height of receipt

    ImVec2 screenSize = ImGui::GetIO().DisplaySize; 

    // 1. ล็อกตำแหน่งให้อยู่ชิดมุมซ้ายบนสุด (X=0, Y=0) และขยายให้เต็มจอ
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always); 
    ImGui::SetNextWindowSize(screenSize, ImGuiCond_Always);
        
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
        if(cardData.haveCard){
            showcard(cardData);
        }
        if(selection.getReturnState() == AppState::PRESET_PAGE){
            printpreset(window,selection);
        }else{
            renderBouquet(window,selection,flowerTextures);
        }

        // 3. กำหนดขนาดของปุ่มที่ต้องการ
        float buttonWidth = 150.0f;
        float buttonHeight = 40.0f;

        // 4. คำนวณหาจุดกึ่งกลางแนวนอน: (ความกว้างของหน้าต่างปัจจุบัน - ความกว้างของปุ่ม) / 2
        ImGui::SetCursorPos(ImVec2((screenSize.x - buttonWidth) * 0.5f , 650.0f));

        // 5. สร้างปุ่ม Check out พร้อมแอบตกแต่งสีปุ่มให้เข้ากับธีมร้านดอกไม้
        ImGui::PushStyleColor(ImGuiCol_Button, COLOR_BUTTON);         // สีปุ่มปกติ (ชมพูตุ่นๆ)
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, COLOR_BUTTON_HOVER); // สีตอนเอาเมาส์ชี้
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, COLOR_BUTTON_ACTIVE);   // สีตอนคลิก
        PushFont(FONT_BODY);
        if (ImGui::Button("Check out", ImVec2(buttonWidth, buttonHeight))) {
            std::cout << "กำลังดำเนินการชำระเงิน...\n";
            currentpages = 1;
        }

        // 6. อย่าลืมคืนค่าสีกลับเป็นปกติให้ระบบด้วยนะคะ (คืนค่า 3 ครั้งตามที่ Push ไป)
        ImGui::PopStyleColor(3);
        PopFont();
        

    }else if(currentpages == 1){
        showreceipt = true;
        ShowReceiptModal(&showreceipt,items,string(cardData.message),receiptheight,selection,container,appstate);
    }
    End();
}