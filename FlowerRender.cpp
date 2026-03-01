#include "FlowerRenderer.h"
#include "config.h"
#include <vector>

void renderBouquet(sf::RenderWindow& window, const UserSelection& selection, const std::vector<sf::Texture>& flowerTextures) {
    // 0.จัดการพื้นหลัง
     sf::VertexArray bg(sf::PrimitiveType::TriangleStrip, 4);
    bg[0].position = {0.f, 0.f};
    bg[1].position = {(float)WINDOW_WIDTH, 0.f};
    bg[2].position = {0.f, (float)WINDOW_HEIGHT};
    bg[3].position = {(float)WINDOW_WIDTH, (float)WINDOW_HEIGHT};

    bg[0].color = BG_GRADIENT_TOP_LEFT;
    bg[1].color = BG_GRADIENT_TOP_RIGHT;
    bg[2].color = BG_GRADIENT_BOTTOM_LEFT;
    bg[3].color = BG_GRADIENT_BOTTOM_RIGHT;

    window.draw(bg);

    
    // 1. จัดการภาชนะ 
    sf::Texture containerTex;
    std::string path = "picture/" + selection.containerType + "-" + selection.containerSize + ".png";
    
    if (containerTex.loadFromFile(path)) {
        sf::Sprite containerSprite(containerTex);
        // กำหนดตำแหน่งที่ภาชนะจะวางบนผืนผ้าใบ (เช่น กึ่งกลางจอ)
        sf::Vector2f canvasCenter(WINDOW_WIDTH *0.75f, WINDOW_HEIGHT / 2.f);
        containerSprite.setScale({ 0.5f, 0.5f });
        containerSprite.setOrigin({ (float)containerTex.getSize().x / 2.f, (float)containerTex.getSize().y / 2.f });
        containerSprite.setPosition(canvasCenter);
        window.draw(containerSprite); // วาดภาชนะก่อนเสมอเพื่อให้เป็นเลเยอร์ล่าง

        // 2. เลือกชุดพิกัดดอกไม้ (สัมพัทธ์กับตำแหน่งภาชนะ)
        std::vector<sf::Vector2f> positions;
        if (selection.containerType == "Vase") {
            if (selection.containerSize == "S") {
                // ใช้พิกัดบวกเพิ่มจากจุดศูนย์กลางภาชนะ (Offset)
                positions = { 
                    canvasCenter + sf::Vector2f(0, -270),   // ดอกกลาง (ขยับขึ้นไปสูงขึ้น)
                    canvasCenter + sf::Vector2f(-65, -240), // ดอกซ้าย
                    canvasCenter + sf::Vector2f(65, -240),  // ดอกขวา
                };
            }
            if (selection.containerSize == "M") {
                // ใช้พิกัดบวกเพิ่มจากจุดศูนย์กลางภาชนะ (Offset)
                positions = { 
                    canvasCenter + sf::Vector2f(0, -270),   // ดอกกลาง (ขยับขึ้นไปสูงขึ้น)
                    canvasCenter + sf::Vector2f(-65, -240), // ดอกซ้าย
                    canvasCenter + sf::Vector2f(65, -240),  // ดอกขวา
                    canvasCenter + sf::Vector2f(-30, -170), // ดอกเสริมซ้าย
                    canvasCenter + sf::Vector2f(30, -170)   // ดอกเสริมขวา
                };
            }
            if (selection.containerSize == "L") {
                // ใช้พิกัดบวกเพิ่มจากจุดศูนย์กลางภาชนะ (Offset)
                positions = { 
                    canvasCenter + sf::Vector2f(0, -270),   // ดอกกลาง (ขยับขึ้นไปสูงขึ้น)
                    canvasCenter + sf::Vector2f(-65, -240), // ดอกซ้าย
                    canvasCenter + sf::Vector2f(65, -240),  // ดอกขวา
                    canvasCenter + sf::Vector2f(-105, -200), // ดอกซ้ายล่าง
                    canvasCenter + sf::Vector2f(105, -200),  // ดอกขวาล่าง
                    canvasCenter + sf::Vector2f(-30, -170), // ดอกเสริมซ้าย
                    canvasCenter + sf::Vector2f(30, -170)   // ดอกเสริมขวา
                };
            }
        }
        if (selection.containerType == "Bouquet") {
            if (selection.containerSize == "S") {
                positions = { 
                    canvasCenter + sf::Vector2f(-10, -280),   // ดอกกลาง (ขยับขึ้นไปสูงขึ้น)
                    canvasCenter + sf::Vector2f(-75, -250), // ดอกซ้าย
                    canvasCenter + sf::Vector2f(55, -250),  // ดอกขวา
                    canvasCenter + sf::Vector2f(-40, -180), // ดอกเสริมซ้าย
                    canvasCenter + sf::Vector2f(20, -180) 
                };
            }
            if (selection.containerSize == "M") {
                positions = { 
                    canvasCenter + sf::Vector2f(-5, -200),   // ดอกกลาง (ขยับขึ้นไปสูงขึ้น)
                    canvasCenter + sf::Vector2f(-85, -165), // ดอกซ้าย
                    canvasCenter + sf::Vector2f(75, -165),  // ดอกขวา
                    canvasCenter + sf::Vector2f(-125, -110), // ดอกซ้ายล่าง
                    canvasCenter + sf::Vector2f(115, -110),  // ดอกขวาล่าง
                    canvasCenter + sf::Vector2f(-5,-135), //ดอกกลางกลาง
                    canvasCenter + sf::Vector2f(-85, -95),// ดอกล่างซ้าย
                    canvasCenter + sf::Vector2f(75, -95), //ดอกล่างขวา
                    canvasCenter + sf::Vector2f(-40, -70), // ดอกเสริมซ้าย
                    canvasCenter + sf::Vector2f(30, -70), // ดอกเสริมขวา
                    
                };
            }
            if (selection.containerSize == "L") {
                positions = { 
                    canvasCenter + sf::Vector2f(-5, -240),   // ดอกกลาง (ขยับขึ้นไปสูงขึ้น)
                    canvasCenter + sf::Vector2f(-85, -205), // ดอกซ้าย
                    canvasCenter + sf::Vector2f(75, -205),  // ดอกขวา
                    canvasCenter + sf::Vector2f(-125, -150), // ดอกซ้ายล่าง
                    canvasCenter + sf::Vector2f(115, -150),  // ดอกขวาล่าง
                    canvasCenter + sf::Vector2f(-5,-170), //ดอกกลางกลาง
                    canvasCenter + sf::Vector2f(-85, -135),// ดอกล่างซ้าย
                    canvasCenter + sf::Vector2f(75, -135), //ดอกล่างขวา
                    canvasCenter + sf::Vector2f(-40, -110), // ดอกเสริมซ้าย
                    canvasCenter + sf::Vector2f(30, -110), // ดอกเสริมขวา
                    canvasCenter + sf::Vector2f(-140, -120), //ดอกซ้ายสุด
                    canvasCenter + sf::Vector2f(130, -120),//ดอกขวาสุด
                    canvasCenter + sf::Vector2f(-80, -90),//ดอกกรองล่างซ้าย
                    canvasCenter + sf::Vector2f(70, -90),//ดอกรองล้่างขวา
                    canvasCenter + sf::Vector2f(-5, -60)//ดอกล่างสุด
                };
            }
        }
        if (selection.containerType == "Basket") {
            if (selection.containerSize == "M") {
                positions = { 
                    canvasCenter + sf::Vector2f(110-7, -90), 
                    canvasCenter + sf::Vector2f(45-7, -90),
                    canvasCenter + sf::Vector2f(-20-7, -90),
                    canvasCenter + sf::Vector2f(-85-7, -90),
                    canvasCenter + sf::Vector2f(142.5-7, -40),
                    canvasCenter + sf::Vector2f(77.5-7, -40),
                    canvasCenter + sf::Vector2f(12.5-7, -40),
                    canvasCenter + sf::Vector2f(-52.5-7, -40),
                    canvasCenter + sf::Vector2f(-117.5-7, -40),
                    canvasCenter + sf::Vector2f(170-7, 10),
                    canvasCenter + sf::Vector2f(110-7, 10),
                    canvasCenter + sf::Vector2f(45-7, 10),
                    canvasCenter + sf::Vector2f(-20-7, 10),
                    canvasCenter + sf::Vector2f(-85-7, 10),
                    canvasCenter + sf::Vector2f(-150-7, 10),
                };
            }
            if (selection.containerSize == "L") {
                positions = { 
                    canvasCenter + sf::Vector2f(77.5-7, -140),
                    canvasCenter + sf::Vector2f(12.5-7, -140),
                    canvasCenter + sf::Vector2f(-52.5-7, -140),
                    canvasCenter + sf::Vector2f(110-7, -90),
                    canvasCenter + sf::Vector2f(45-7, -90),
                    canvasCenter + sf::Vector2f(-20-7, -90),
                    canvasCenter + sf::Vector2f(-85-7, -90),
                    canvasCenter + sf::Vector2f(142.5-7, -40),
                    canvasCenter + sf::Vector2f(77.5-7, -40),
                    canvasCenter + sf::Vector2f(12.5-7, -40),
                    canvasCenter + sf::Vector2f(-52.5-7, -40),
                    canvasCenter + sf::Vector2f(-117.5-7, -40),
                    canvasCenter + sf::Vector2f(170-7, 10),
                    canvasCenter + sf::Vector2f(110-7, 10),
                    canvasCenter + sf::Vector2f(45-7, 10),
                    canvasCenter + sf::Vector2f(-20-7, 10),
                    canvasCenter + sf::Vector2f(-85-7, 10),
                    canvasCenter + sf::Vector2f(-150-7, 10),
                    canvasCenter + sf::Vector2f(77.5-7, 60),
                    canvasCenter + sf::Vector2f(12.5-7, 60),
                    canvasCenter + sf::Vector2f(-52.5-7, 60) 
                };
            }
        }
        // 3. วาดดอกไม้ทับด้านบน (เลเยอร์บน)
        for (size_t i = 0; i < selection.selectedFlowers.size() && i < positions.size(); ++i) {
            const auto& flower = selection.selectedFlowers[i];
            sf::Sprite flowerSprite(flowerTextures[(int)flower.type]);
            
            flowerSprite.setOrigin({flowerSprite.getLocalBounds().size.x / 2.f, flowerSprite.getLocalBounds().size.y / 2.f});
            flowerSprite.setPosition(positions[i]);
            flowerSprite.setScale({ 0.12f, 0.12f });

            window.draw(flowerSprite); // ดอกไม้จะทับบนภาชนะเพราะวาดทีหลัง
        }
    }
}