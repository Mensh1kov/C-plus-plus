#pragma once
#include "checker.h"
#include <map>

enum HowMove {from_cell_err, in_cell_err, fell_err, successfully, can_move_again, move_transition, winner};

class Board {
    Color first_color;
    Color second_color;

    Player *move_player;
    Player *sleep_player;

    std::map<std::pair<int, int>, Checker *> board;
public:
    Board(Color first_color, Color second_color, Player &first_player, Player &second_player);
    bool FindBoard(const std::pair<int, int> key) const { return board.find(key) != board.end(); }
    void EditMovePlayer() { std::swap(move_player, sleep_player); }
    void CheckBoard();
    HowMove Move(std::pair<int, int> from, std::pair<int, int> in);
    std::map<std::pair<int, int>, Checker *> &GetBoard() { return board; }
    Player &GetMovePlayer() { return *move_player; }
    Player &GetSleepPlayer() { return *sleep_player; }
    Color GetFirstColor() const { return first_color; }
    Color GetSecondColor() const { return second_color; }
};