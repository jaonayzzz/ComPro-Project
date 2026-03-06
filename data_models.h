#pragma once
#include <string>
#include <vector>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics.hpp>
#include "app_state.h"

enum class EntrySource {
    NONE,
    PRESET,
    RANDOM,
    CUSTOM
};

enum class FlowerType {
    Lotus,
    WhiteCarnation,
    WhiteChrysanthemum,
    WhiteRose,
    WhiteOrchid,
    WhiteTulip,
    RedPoppy,
    WhiteLily,
    WhiteHydrangea,
    WhiteAnemone,
    Marigold,
    WhiteDaisy,
    YellowDaisy,
    Sunflower,
    PinkCarnation,
    YellowChrysanthemum,
    YellowRose,
    PurpleDahlia,
    PurpleOrchid,
    YellowTulip,
    PurpleIris,
    RedPeony,
    Jasmine,
    RedCarnation,
    RedRose,
    PinkRose,
    RedDahlia,
    Lavender,
    Chamomile,
    RedTulip,
    ForgetMeNot,
    PinkLily,
    BlueHydrangea,
    PinkHydrangea,
    RedAnemone,
    PinkPeony
};

struct Flower {
    int id;
    FlowerType type;
    std::string name;
    std::string meaning;
    int price;
    std::string occasion;
    std::string imagePath;
    sf::Texture texture; // เพิ่มตัวนี้เข้ามาเพื่อเก็บรูปที่โหลดแล้ว
};

struct Container {
    std::string type;
    std::string size;
    int minF;
    int maxF;
    int basePrice;
    int minTotal;
    int maxTotal;
};


struct UserSelection {

    std::string occasion;
    std::string containerType;
    std::string containerSize;
    bool isCustomMode;

    EntrySource source = EntrySource::NONE;
    std::string presetName;
    std::string presetImagePath;
    std::vector<std::string> flowers;

    std::vector<Flower> selectedFlowers; // เก็บดอกไม้ที่สุ่มได้หรือเลือกเอง
    int totalAmount = 0;

    AppState getReturnState() const
    {
        switch (source)
        {
            case EntrySource::CUSTOM: return AppState::CUSTOM_MODE;
            case EntrySource::RANDOM: return AppState::RANDOM_MODE;
            case EntrySource::PRESET: return AppState::PRESET_PAGE;
            default: return AppState::MAIN_MENU;
        }
    }
};

struct BudgetRange {
    int min;
    int max;
};

struct FlowerPreset
{
    std::string name;
    std::string occasion;
    std::string containerType;
    std::string containerSize;
    int price;
    std::string imagePath;
    std::vector<std::string> flowers;
    sf::Texture texture;
};

extern std::vector<FlowerPreset> developerPresets;
extern std::vector<Flower> flowerList;
extern std::vector<Container> containerList;

BudgetRange calculateActualBudget(const std::string& occasion, const Container& container);

struct OrderCardData {
    bool haveCard;
    char recipient[64] = "";
    char sender[64] = "";
    char message[100] = "";
    //sf::Texture coverImage;
    void Clear() {
        message[0] = '\0';    // เคลียร์ข้อความให้กลายเป็น String ว่าง
        recipient[0] = '\0'; // เคลียร์ชื่อคนส่ง
        sender[0] = '\0';        // รีเซ็ตสไตล์การ์ด
    }
};