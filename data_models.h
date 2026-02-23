#pragma once
#include <string>
#include <vector>
#include <SFML/Graphics/Color.hpp>

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
    int price;
    std::string occasion;
    sf::Color primaryColor;
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
};

extern std::vector<Flower> flowerList;
extern std::vector<Container> containerList;