#pragma once
#include "board.h"

void Print(auto data, const Color & color_text, const Color & color_back);

class Game {
    Board * board;
public:
    Game();
    Board * GetBoard() {return board;}
    HowMove GetMove();
    void PrintBoard();
    void Error(HowMove code);
    void UpdateField();
    void Winner();
};