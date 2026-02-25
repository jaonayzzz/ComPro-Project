#include "FlowerRenderer.h"
#include <vector>

void renderBouquet(sf::RenderWindow& window, const UserSelection& selection, const std::vector<sf::Texture>& flowerTextures) {
    // 1. จัดการภาชนะ (เลเยอร์ล่างสุด)
    sf::Texture containerTex;
    std::string path = "picture/" + selection.containerType + "-" + selection.containerSize + ".png";
    
    if (containerTex.loadFromFile(path)) {
        sf::Sprite containerSprite(containerTex);
        // กำหนดตำแหน่งที่ภาชนะจะวางบนผืนผ้าใบ (เช่น กึ่งกลางจอ)
        sf::Vector2f canvasCenter(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f);
        
        containerSprite.setOrigin({ (float)containerTex.getSize().x / 2.f, (float)containerTex.getSize().y / 2.f });
        containerSprite.setPosition(canvasCenter);
        window.draw(containerSprite); // วาดภาชนะก่อนเสมอเพื่อให้เป็นเลเยอร์ล่าง

        // 2. เลือกชุดพิกัดดอกไม้ (สัมพัทธ์กับตำแหน่งภาชนะ)
        std::vector<sf::Vector2f> positions;
        if (selection.containerType == "Vase") {
            if (selection.containerSize == "S") {
                // ใช้พิกัดบวกเพิ่มจากจุดศูนย์กลางภาชนะ (Offset)
                positions = { 
                    canvasCenter + sf::Vector2f(0, -50), 
                    canvasCenter + sf::Vector2f(-30, -70), 
                    canvasCenter + sf::Vector2f(30, -70) 
                };
            }
            if (selection.containerSize == "M") {
                // ใช้พิกัดบวกเพิ่มจากจุดศูนย์กลางภาชนะ (Offset)
                positions = { 
                    canvasCenter + sf::Vector2f(0, -50), 
                    canvasCenter + sf::Vector2f(-30, -70), 
                    canvasCenter + sf::Vector2f(30, -70),
                    canvasCenter + sf::Vector2f(-50, -70),
                    canvasCenter + sf::Vector2f(50, -70) 
                };
            }
            if (selection.containerSize == "L") {
                // ใช้พิกัดบวกเพิ่มจากจุดศูนย์กลางภาชนะ (Offset)
                positions = { 
                    canvasCenter + sf::Vector2f(0, -50), 
                    canvasCenter + sf::Vector2f(-30, -70), 
                    canvasCenter + sf::Vector2f(30, -70),
                    canvasCenter + sf::Vector2f(-50, -70),
                    canvasCenter + sf::Vector2f(50, -70),
                    canvasCenter + sf::Vector2f(-40, -60),
                    canvasCenter + sf::Vector2f(40, -60) 
                };
            }
        }
        if (selection.containerType == "Bouquet") {
            if (selection.containerSize == "S") {
                positions = { 
                    canvasCenter + sf::Vector2f(0, -50), 
                    canvasCenter + sf::Vector2f(-30, -70), 
                    canvasCenter + sf::Vector2f(30, -70),
                    canvasCenter + sf::Vector2f(-50, -70),
                    canvasCenter + sf::Vector2f(50, -70),
                    canvasCenter + sf::Vector2f(40, -60) 
                };
            }
            if (selection.containerSize == "M") {
                positions = { 
                    canvasCenter + sf::Vector2f(0, -50), 
                    canvasCenter + sf::Vector2f(-30, -70), 
                    canvasCenter + sf::Vector2f(30, -70),
                    canvasCenter + sf::Vector2f(-50, -70),
                    canvasCenter + sf::Vector2f(50, -70),
                    canvasCenter + sf::Vector2f(-40, -60),
                    canvasCenter + sf::Vector2f(40, -60),
                    canvasCenter + sf::Vector2f(-70, -60),
                    canvasCenter + sf::Vector2f(70, -60),
                    canvasCenter + sf::Vector2f(90, -70)  
                };
            }
            if (selection.containerSize == "L") {
                positions = { 
                    canvasCenter + sf::Vector2f(0, -50), 
                    canvasCenter + sf::Vector2f(-30, -70), 
                    canvasCenter + sf::Vector2f(30, -70),
                    canvasCenter + sf::Vector2f(-50, -70),
                    canvasCenter + sf::Vector2f(50, -70),
                    canvasCenter + sf::Vector2f(-40, -60),
                    canvasCenter + sf::Vector2f(40, -60),
                    canvasCenter + sf::Vector2f(-70, -60),
                    canvasCenter + sf::Vector2f(70, -60),
                    canvasCenter + sf::Vector2f(-90, -70),
                    canvasCenter + sf::Vector2f(90, -70),
                    canvasCenter + sf::Vector2f(-80, -60),
                    canvasCenter + sf::Vector2f(80, -60),
                    canvasCenter + sf::Vector2f(-30, -30),
                    canvasCenter + sf::Vector2f(30, -30)
                };
            }
        }
        if (selection.containerType == "Basket") {
            if (selection.containerSize == "M") {
                positions = { 
                    canvasCenter + sf::Vector2f(-60, -50), 
                    canvasCenter + sf::Vector2f(-40, -50), 
                    canvasCenter + sf::Vector2f(-20, -50),
                    canvasCenter + sf::Vector2f(0, -50),
                    canvasCenter + sf::Vector2f(20, -50),
                    canvasCenter + sf::Vector2f(40, -50),
                    canvasCenter + sf::Vector2f(-50, -40),
                    canvasCenter + sf::Vector2f(-30, -40),
                    canvasCenter + sf::Vector2f(-10, -40),
                    canvasCenter + sf::Vector2f(10, -40),
                    canvasCenter + sf::Vector2f(30, -40),
                    canvasCenter + sf::Vector2f(-40, -30),
                    canvasCenter + sf::Vector2f(-20, -30),
                    canvasCenter + sf::Vector2f(0, -30),
                    canvasCenter + sf::Vector2f(20, -30) 
                };
            }
            if (selection.containerSize == "L") {
                positions = { 
                    canvasCenter + sf::Vector2f(-70, -60),
                    canvasCenter + sf::Vector2f(-50, -60),
                    canvasCenter + sf::Vector2f(-30, -60),
                    canvasCenter + sf::Vector2f(-10, -60),
                    canvasCenter + sf::Vector2f(10, -60),
                    canvasCenter + sf::Vector2f(30, -60),
                    canvasCenter + sf::Vector2f(50, -60),
                    canvasCenter + sf::Vector2f(-60, -50), 
                    canvasCenter + sf::Vector2f(-40, -50), 
                    canvasCenter + sf::Vector2f(-20, -50),
                    canvasCenter + sf::Vector2f(0, -50),
                    canvasCenter + sf::Vector2f(20, -50),
                    canvasCenter + sf::Vector2f(40, -50),
                    canvasCenter + sf::Vector2f(-50, -40),
                    canvasCenter + sf::Vector2f(-30, -40),
                    canvasCenter + sf::Vector2f(-10, -40),
                    canvasCenter + sf::Vector2f(10, -40),
                    canvasCenter + sf::Vector2f(30, -40),
                    canvasCenter + sf::Vector2f(-40, -30),
                    canvasCenter + sf::Vector2f(-20, -30),
                    canvasCenter + sf::Vector2f(0, -30),
                    canvasCenter + sf::Vector2f(20, -30),
                    canvasCenter + sf::Vector2f(0, -20) 
                };
            }
        }
        // 3. วาดดอกไม้ทับด้านบน (เลเยอร์บน)
        for (size_t i = 0; i < selection.selectedFlowers.size() && i < positions.size(); ++i) {
            const auto& flower = selection.selectedFlowers[i];
            sf::Sprite flowerSprite(flowerTextures[(int)flower.type]);
            
            flowerSprite.setOrigin({flowerSprite.getLocalBounds().size.x / 2.f, flowerSprite.getLocalBounds().size.y / 2.f});
            flowerSprite.setPosition(positions[i]);
            flowerSprite.setScale({ 0.5f, 0.5f });

            window.draw(flowerSprite); // ดอกไม้จะทับบนภาชนะเพราะวาดทีหลัง
        }
    }
}