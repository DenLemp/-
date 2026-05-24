#pragma once
#include <vector>

enum class D20Mode { Normal, Advantage, Disadvantage };

struct RollResult {
    std::vector<int> dice;
    int total = 0;
    int dropped = 0; // for 4d6 drop lowest
    bool hasDropped = false;
};

class DiceRoller {
public:
    static int roll(int sides);
    static RollResult rollMany(int sides, int count);
    static RollResult rollD20(D20Mode mode);
    static RollResult rollAbilityScore(); // 4d6 drop lowest
};
