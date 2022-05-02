#include "checker.h"

Checker::Checker(std::pair<int, int> position, Color color_checker, Orientation orientation,  char symbol_checker, char symbol_king) : color_checker(color_checker), orientation(orientation) {
    this->position = position;
    this->symbol_checker = symbol_checker;
    this->symbol_king = symbol_king;
}
std::pair<int, int> Checker::RightMove() const {
    int x = this->position.first + 1 * orientation;
    int y = this->position.second + 1;
    return {x, y};
}
std::pair<int, int> Checker::LeftMove() const {
    int x = this->position.first + 1 * orientation;
    int y = this->position.second - 1;
    return {x, y};
}