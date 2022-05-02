#pragma once
#include "player.h"
#include <iostream>
#include <map>
#include <windows.h>

enum Orientation {first_player = 1, second_player = -1};

class Checker {
    std::pair<int, int> position;
    const Color color_checker;
    bool king = false;
    bool can_fell = false;
    std::string symbol_checker;
    std::string symbol_king;
    std::map<std::pair<int, int>, std::pair<int, int>> can_move = {};
    const int orientation; // ?
public:
    Checker(std::pair<int, int> position, Color color_checker,  Orientation orientation, char symbol_checker, char symbol_king);

    void EditPosition(std::pair<int, int> & new_position) {this->position = new_position;}
    void EditKing(bool new_king) {this->king = new_king;}
    void EditCanFell(bool new_can_fell) {this->can_fell = new_can_fell;}
    void EditCanMove(std::map<std::pair<int, int>, std::pair<int, int>> & new_can_move) {this->can_move = new_can_move;}

    std::pair<int, int> GetPosition() const {return position;}
    Color GetColor() const {return color_checker;}
    bool GetKing() const {return king;}
    bool GetCanFell() const {return can_fell;}
    bool FindCanMove(const std::pair<int,int> key) const {return can_move.find(key) != can_move.end();}
    std::map<std::pair<int, int>, std::pair<int, int>> & GetCanMove() {return can_move;}
    const int & GetOrientation() {return orientation;}
    std::string & GetSymChecker() {return symbol_checker;}
    std::string & GetSymKing() {return symbol_king;}

    std::pair<int, int> RightMove() const;
    std::pair<int, int> LeftMove() const;
};