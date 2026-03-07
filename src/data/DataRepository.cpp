#include "DataModels.h"
#include <vector>
#include <algorithm>

std::vector<Flower> flowerList = {
    {1,  FlowerType::Lotus,              "Lotus",              "Peace Purity Spirituality",          20,  "Condolences", "../assets/picture/Lotus.png"},
    {2,  FlowerType::WhiteCarnation,     "White Carnation",    "Respect Remembrance",                40,  "Condolences", "../assets/picture/White-Carnation.png"},
    {3,  FlowerType::WhiteChrysanthemum, "White Chrysanthemum","Condolence Respect Peace",           40,  "Condolences", "../assets/picture/White-Chrysanthemum.png"},
    {4,  FlowerType::WhiteRose,          "White Rose",         "Purity Sincerity Respect",           50,  "Condolences", "../assets/picture/White-Rose.png"},
    {5,  FlowerType::WhiteOrchid,        "White Orchid",       "Elegance Sincerity",                 80,  "Condolences", "../assets/picture/White-Orchid.png"},
    {6,  FlowerType::WhiteTulip,         "White Tulip",        "Forgiveness Peace",                  100, "Condolences", "../assets/picture/White-Tulip.png"},
    {7,  FlowerType::RedPoppy,           "Red Poppy",          "Remembrance Bravery",                100, "Condolences", "../assets/picture/Red-Poppy.png"},
    {8,  FlowerType::WhiteLily,          "White Lily",         "Purity Elegance Respect",            120, "Condolences", "../assets/picture/White-Lily.png"},
    {9,  FlowerType::WhiteHydrangea,     "White Hydrangea",    "Sincerity Elegance",                 150, "Condolences", "../assets/picture/White-Hydrangea.png"},
    {10, FlowerType::WhiteAnemone,       "White Anemone",      "Sincerity Calmness",                 150, "Condolences", "../assets/picture/White-Anemone.png"},
    {11, FlowerType::Marigold,           "Marigold",           "Prosperity Luck Success",            15,  "Congratulations", "../assets/picture/Marigold.png"},
    {12, FlowerType::WhiteDaisy,         "White Daisy",        "Innocence Kindness New beginnings",  25,  "Congratulations", "../assets/picture/White-Daisy.png"},
    {13, FlowerType::YellowDaisy,        "Yellow Daisy",       "Cheerfulness Friendship",            25,  "Congratulations", "../assets/picture/Yellow-Daisy.png"},
    {14, FlowerType::Sunflower,          "Sunflower",          "Positivity Hope Loyal love",         40,  "Congratulations", "../assets/picture/Sunflower.png"},
    {15, FlowerType::PinkCarnation,      "Pink Carnation",     "Gratitude Affection",                40,  "Congratulations", "../assets/picture/Pink-Carnation.png"},
    {16, FlowerType::YellowChrysanthemum,"Yellow Chrysanthemum","Encouragement Longevity Good wishes", 40, "Congratulations", "../assets/picture/Yellow-Chrysanthemum.png"},
    {17, FlowerType::YellowRose,         "Yellow Rose",        "Friendship Joy Encouragement",       50,  "Congratulations", "../assets/picture/Yellow-Rose.png"},
    {18, FlowerType::PurpleDahlia,       "Purple Dahlia",      "Elegance Mystery Charm",             60,  "Congratulations", "../assets/picture/Purple-Dahlia.png"},
    {19, FlowerType::PurpleOrchid,       "Purple Orchid",      "Luxury Respect Charm",               80,  "Congratulations", "../assets/picture/Purple-Orchid.png"},
    {20, FlowerType::YellowTulip,        "Yellow Tulip",       "Happiness Cheerfulness",             100, "Congratulations", "../assets/picture/Yellow-Tulip.png"},
    {21, FlowerType::PurpleIris,         "Purple Iris",        "Hope Good news Trust",               120, "Congratulations", "../assets/picture/Purple-Iris.png"},
    {22, FlowerType::RedPeony,           "Red Peony",          "Prosperity Honor Good fortune",      250, "Congratulations", "../assets/picture/Red-Peony.png"},
    {23, FlowerType::Jasmine,            "Jasmine",            "Pure love Gratitude",                10,  "Love & Care", "../assets/picture/Jasmine.png"},
    {24, FlowerType::RedCarnation,       "Red Carnation",      "Deep love Admiration",               40,  "Love & Care", "../assets/picture/Red-Carnation.png"},
    {25, FlowerType::RedRose,            "Red Rose",           "True love Romance Passion",          50,  "Love & Care", "../assets/picture/Red-Rose.png"},
    {26, FlowerType::PinkRose,           "Pink Rose",          "Sweetness Admiration Gratitude",     50,  "Love & Care", "../assets/picture/Pink-Rose.png"},
    {27, FlowerType::RedDahlia,          "Red Dahlia",         "Strength Lasting love",              60,  "Love & Care", "../assets/picture/Red-Dahlia.png"},
    {28, FlowerType::Lavender,           "Lavender",           "Calmness Relaxation Gentle love",    80,  "Love & Care", "../assets/picture/Lavender.png"},
    {29, FlowerType::Chamomile,          "Chamomile",          "Comfort Calm Gentleness",            80,  "Love & Care", "../assets/picture/Chamomile.png"},
    {30, FlowerType::RedTulip,           "Red Tulip",          "True love Confession",               100, "Love & Care", "../assets/picture/Red-Tulip.png"},
    {31, FlowerType::ForgetMeNot,        "Forget-me-not",      "Remembrance True memories",          100, "Love & Care", "../assets/picture/Forget-me-not.png"},
    {32, FlowerType::PinkLily,           "Pink Lily",          "Love Tenderness",                    120, "Love & Care", "../assets/picture/Pink-Lily.png"},
    {33, FlowerType::BlueHydrangea,      "Blue Hydrangea",     "Understanding Deep emotions",        150, "Love & Care", "../assets/picture/Blue-Hydrangea.png"},
    {34, FlowerType::PinkHydrangea,      "Pink Hydrangea",     "Love Sweetness Warmth",              150, "Love & Care", "../assets/picture/Pink-Hydrangea.png"},
    {35, FlowerType::RedAnemone,         "Red Anemone",        "Deep love Strong feelings",          150, "Love & Care", "../assets/picture/Red-Anemone.png"},
    {36, FlowerType::PinkPeony,          "Pink Peony",         "Romance Happy love",                 250, "Love & Care", "../assets/picture/Pink-Peony.png"}
};

std::vector<Container> containerList = {
    {"Vase", "S", 1, 3, 29, 0, 0},
    {"Vase", "M", 4, 5, 69, 0, 0},
    {"Vase", "L", 6, 7, 99, 0, 0},
    {"Bouquet", "S", 3, 6, 19, 0, 0},
    {"Bouquet", "M", 7, 10, 29, 0, 0},
    {"Bouquet", "L", 11, 15, 39, 0, 0},
    {"Basket", "M", 10, 15, 69, 0, 0},
    {"Basket", "L", 16, 23, 119, 0, 0}
};

BudgetRange calculateActualBudget(const std::string& occasion, const Container& container) {
    std::vector<int> prices;
    
    for (const auto& f : flowerList) {
        if (f.occasion == occasion) {
            prices.push_back(f.price);
        }
    }

    if (prices.empty()) return { container.basePrice, container.basePrice };

    std::sort(prices.begin(), prices.end());

    int minTotal = container.basePrice + (prices.front() * container.minF);
    int maxTotal = container.basePrice + (prices.back() * container.maxF);

    return { minTotal, maxTotal };
}

std::vector<FlowerPreset> developerPresets = {
    {
        "Just Pink",
        "Love & Care",
        "Bouquet",
        "L",
        1149,
        "../assets/picture/Pao_Preset.png",
        {
            "Pink Carnation", "Pink Carnation", "Pink Carnation", "Pink Carnation",
            "Pink Lily", "Pink Lily", "Pink Lily", "Pink Lily",
            "Pink Hydrangea", "Pink Hydrangea", "Pink Hydrangea"
        }
    },
    {
        "Just love u",
        "Love & Care",
        "Bouquet",
        "L",
        1389,
        "../assets/picture/Am_Preset.png",
        {
            "Sunflower", "Forget-me-not", "Forget-me-not", "Pink Rose", 
            "Blue Hydrangea", "White Rose", "Pink Peony", "Pink Hydrangea", 
            "Pink Carnation", "White Carnation"
        }
    },
    {
        "LaBlanc",
        "Condolences",
        "Vase",
        "L",
        1499,
        "../assets/picture/Nay_Preset.png",
        {
            "White Hydrangea", "White Hydrangea", "White Hydrangea", 
            "Lotus", "White Carnation", "White Carnation", "White Carnation", 
            "Red Poppy", "Red Poppy", "Red Poppy", "White Lily"
        }
    },
        {
        "1DerFull",
        "Love & Care",
        "Bouquet",
        "L",
        1259,
        "../assets/picture/Chom_Preset.png",
        {
            "Pink Hydrangea", "Pink Hydrangea", "Pink Hydrangea", "Pink Hydrangea", "Pink Hydrangea",
            "Pink Lily", "Pink Lily", "Pink Lily", "Pink Lily", "Pink Lily",
            "Lavender", "Lavender", "Sunflower"
        }
    }
};