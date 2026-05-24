#include "DiceRoller.h"
#include <algorithm>
#include <random>

namespace {
std::mt19937& rng() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return gen;
}
} // namespace

int DiceRoller::roll(int sides) {
    std::uniform_int_distribution<int> dist(1, sides);
    return dist(rng());
}

RollResult DiceRoller::rollMany(int sides, int count) {
    RollResult r;
    r.dice.reserve(static_cast<size_t>(count));
    for (int i = 0; i < count; ++i) {
        int v = roll(sides);
        r.dice.push_back(v);
        r.total += v;
    }
    return r;
}

RollResult DiceRoller::rollD20(D20Mode mode) {
    RollResult r;
    if (mode == D20Mode::Normal) {
        int v = roll(20);
        r.dice = {v};
        r.total = v;
        return r;
    }
    int a = roll(20);
    int b = roll(20);
    r.dice = {a, b};
    r.total = (mode == D20Mode::Advantage) ? std::max(a, b) : std::min(a, b);
    return r;
}

RollResult DiceRoller::rollAbilityScore() {
    RollResult r = rollMany(6, 4);
    r.dropped = *std::min_element(r.dice.begin(), r.dice.end());
    r.hasDropped = true;
    r.total = 0;
    bool removed = false;
    for (int v : r.dice) {
        if (!removed && v == r.dropped) {
            removed = true;
            continue;
        }
        r.total += v;
    }
    return r;
}
