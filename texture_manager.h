#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include "data_models.h"

// ดึงตัวแปร flowerList มาจาก data_repository.cpp
extern std::vector<Flower> flowerList; 

class TextureManager {
public:
    // ใช้ map เพื่อจับคู่ Flower ID กับ Texture
    static std::map<int, sf::Texture>& getFlowerTextures() {
        static std::map<int, sf::Texture> textureMap;
        
        // ถ้ายังไม่ได้โหลด ให้โหลดตามรายชื่อใน flowerList
        if (textureMap.empty()) {
            for (const auto& flower : flowerList) {
                sf::Texture t;
                if (t.loadFromFile(flower.imagePath)) {
                    t.setSmooth(true);
                    t.generateMipmap();
                    textureMap[flower.id] = std::move(t); // เก็บโดยใช้ ID เป็น Key
                } else {
                    // ถ้าโหลดรูปไม่ได้ จะแจ้งเตือน
                    printf("Warning: Cannot load %s\n", flower.imagePath.c_str());
                }
            }
        }
        return textureMap;
    }
};