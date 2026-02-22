#include <vector>
#include <string>

using namespace std;

struct Flower {
    int id;
    string name;
    string meaning;
    int price;
    string occasion;
};

struct Container {
    string type;
    string size;
    int minF;
    int maxF;
    int basePrice;
    int minTotal;
    int maxTotal;
};

vector<Flower> flowerList = {
    // หมวด Condolences (ไว้อาลัย)
    {1, "Lotus", "Peace Purity Spirituality", 20, "Condolences"},
    {2, "White Carnation", "Respect Remembrance", 40, "Condolences"},
    {3, "White Chrysanthemum", "Condolence Respect Peace", 40, "Condolences"},
    {4, "White Rose", "Purity Sincerity Respect", 50, "Condolences"},
    {5, "White Orchid", "Elegance Sincerity", 80, "Condolences"},
    {6, "White Tulip", "Forgiveness Peace", 100, "Condolences"},
    {7, "Red Poppy", "Remembrance Bravery", 100, "Condolences"},
    {8, "White Lily", "Purity Elegance Respect", 120, "Condolences"},
    {9, "White Hydrangea", "Sincerity Elegance", 150, "Condolences"},
    {10, "White Anemone", "Sincerity Calmness", 150, "Condolences"},

    // หมวด Congratulations (แสดงความยินดี)
    {11, "Marigold", "Prosperity Luck Success", 15, "Congratulations"},
    {12, "White Daisy", "Innocence Kindness New beginnings", 25, "Congratulations"},
    {13, "Yellow Daisy", "Cheerfulness Friendship", 25, "Congratulations"},
    {14, "Sunflower", "Positivity Hope Loyal love", 40, "Congratulations"},
    {15, "Pink Carnation", "Gratitude Affection", 40, "Congratulations"},
    {16, "Yellow Chrysanthemum", "Encouragement Longevity Good wishes", 40, "Congratulations"},
    {17, "Yellow Rose", "Friendship Joy Encouragement", 50, "Congratulations"},
    {18, "Purple Dahlia", "Elegance Mystery Charm", 60, "Congratulations"},
    {19, "Purple Orchid", "Luxury Respect Charm", 80, "Congratulations"},
    {20, "Yellow Tulip", "Happiness Cheerfulness", 100, "Congratulations"},
    {21, "Purple Iris", "Hope Good news Trust", 120, "Congratulations"},
    {22, "Red Peony", "Prosperity Honor Good fortune", 250, "Congratulations"},

    // หมวด Love & Care (ความรักและความห่วงใย)
    {23, "Jasmine", "Pure love Gratitude", 10, "Love & Care"},
    {24, "Red Carnation", "Deep love Admiration", 40, "Love & Care"},
    {25, "Red Rose", "True love Romance Passion", 50, "Love & Care"},
    {26, "Pink Rose", "Sweetness Admiration Gratitude", 50, "Love & Care"},
    {27, "Red Dahlia", "Strength Lasting love", 60, "Love & Care"},
    {28, "Lavender", "Calmness Relaxation Gentle love", 80, "Love & Care"},
    {29, "Chamomile", "Comfort Calm Gentleness", 80, "Love & Care"},
    {30, "Red Tulip", "True love Confession", 100, "Love & Care"},
    {31, "Forget-me-not", "Remembrance True memories", 100, "Love & Care"},
    {32, "Pink Lily", "Love Tenderness", 120, "Love & Care"},
    {33, "Blue Hydrangea", "Understanding Deep emotions", 150, "Love & Care"},
    {34, "Pink Hydrangea", "Love Sweetness Warmth", 150, "Love & Care"},
    {35, "Red Anemone", "Deep love Strong feelings", 150, "Love & Care"},
    {36, "Pink Peony", "Romance Happy love", 250, "Love & Care"}
};

vector<Container> containerList = {
    {"Vase", "S", 1, 3, 29, 39, 779},
    {"Vase", "M", 4, 5, 69, 109, 1319},
    {"Vase", "L", 6, 7, 99, 159, 1849},
    {"Bouquet", "S", 3, 6, 19, 49, 1519},
    {"Bouquet", "M", 7, 10, 29, 99, 2529},
    {"Bouquet", "L", 11, 15, 39, 149, 3789},
    {"Basket", "M", 10, 15, 69, 169, 3819},
    {"Basket", "L", 16, 23, 119, 279, 5869}
};