#pragma once

#include <array>
#include <stdexcept>
#include <queue>
#include <cmath>
enum class Winner { kFirst, kSecond, kNone };

struct GameResult {
    Winner winner;
    int turn;
};

bool WinCard(int& a, int& b) {
    if (a == 0 && b == 11) {
        return true;  // Winner is first
    } else if (a == 11 && b == 0) {
        return false;
    }
    return (a > b);
}
GameResult SimulateWarGame(const std::array<int, 6>& first_deck,
                           const std::array<int, 6>& second_deck) {
    std::queue<int> first(std::deque<int>(first_deck.begin(), first_deck.end()));
    std::queue<int> second(std::deque<int>(second_deck.begin(), second_deck.end()));
    GameResult ans{Winner::kNone, 0};
    int card1, card2;
    while (ans.turn < std::pow(10, 6) && ans.winner == Winner::kNone) {
        if (first.empty()) {
            ans.winner = Winner::kSecond;
        } else if (second.empty()) {
            ans.winner = Winner::kFirst;
        } else {
            card1 = first.front();
            card2 = second.front();
            first.pop();
            second.pop();
            if (WinCard(card1, card2)) {
                first.push(card1);
                first.push(card2);
            } else {
                second.push(card1);
                second.push(card2);
            }
            ans.turn += 1;
        }
    }
    return ans;
}
