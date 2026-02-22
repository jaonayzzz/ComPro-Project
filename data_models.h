#pragma once
//
#include <string>
#include <vector>

struct Flower {
    int id;
    std::string name;
    std::string meaning;
    int price;
    std::string occasion;
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
    int budget;
    bool isCustomMode;
};

extern std::vector<Flower> flowerList;
extern std::vector<Container> containerList;
