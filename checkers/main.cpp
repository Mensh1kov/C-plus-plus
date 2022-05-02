#include <iostream>
#include "game.h"

int main() {
    std::cout << "test commit" << std::endl;
    Game NewGame;
    NewGame.UpdateField();
    while (true) {
        NewGame.PrintBoard();
        HowMove code = NewGame.GetMove();
        switch(code) {
            case move_transition:
                NewGame.UpdateField();
                break;
            case can_move_again:
                break;
            case fell_err:
            case in_cell_err:
            case from_cell_err:
                NewGame.Error(code);
                break;
            case winner:
                NewGame.PrintBoard();
                NewGame.Winner();
                return 0;
            default:
                std::cout << "Error: game process!" << std::endl;
                return -1;
        }
    }
}