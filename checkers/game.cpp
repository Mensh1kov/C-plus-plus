#include "game.h"

void Print(auto data, const Color & color_text, const Color & color_back) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),((color_back << 4) | color_text));
    std::cout << data;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
};

void Game::Error(HowMove code) {
    std::string message;
    switch (code) {
        case from_cell_err:
            message = "Attention! Choose a cell with your checker!";
            break;
        case in_cell_err:
            message = "Attention! You're making the wrong move!";
            break;
        case fell_err:
            message = "Attention! You must cut down the opponent's checker!";
            break;
    }
    Print(message, red, black);
    std::cout << std::endl;
}
void PrintHello() {
    for (int i = 0; i < 12; i++) {
        if (i % 2 == 0)
            Print("   ", brown, brown);
        else
            Print("   ", brown, yellow);
    }
}
void Hello() {
    PrintHello();
    std::cout << std::endl;
    Print("   ", yellow, yellow);
    Print("           CHECKERS           ", light_blue_three, light_gray);
    Print("   ", brown, brown);
    std::cout << std::endl;
    PrintHello();
    std::cout << "\n" << std::endl;
}
void Juggernaut(bool effect, Player & player) {
    for (auto i : player.GetCheckers())
        i->EditKing(effect);
}
void Cheats(std::string & cheat_code, Board * board) {
    board->GetMovePlayer().EditCanFell(false);
    if (cheat_code == "\\juggernaut_on")
        Juggernaut(true, board->GetMovePlayer());
    else if (cheat_code == "\\juggernaut_off")
        Juggernaut(false, board->GetMovePlayer());
    else {
        Print("Unknown command!", red, black);
        std::cout << std::endl;
    }
}
void PrintColor() {
    std::cout << "         ";
    for (int i = 0; i < 16; i++) {
        std::cout << i + 1 << "-";
        Print ("   ", black, static_cast<Color>(i));
        std::cout << " ";
    }
    std::cout << std::endl;
}
Color InputColor() {
    Print("         Enter the number of the color selected from the list: ", light_blue_two, black);
    int num_color;
    std::string input;
    std::cin >> input;
    try {
        num_color = std::stoi(input);
        num_color--;
        if (num_color < 0 || num_color > 15)
            std::stoi("err");
    } catch(std::invalid_argument & err) {
        Print("         Attention! Choose a color from the list!", red, black);
        std::cout << std::endl;
        return InputColor();
    } catch (std::out_of_range & err) {
        Print("         Attention! Choose a color from the list!", red, black);
        std::cout << std::endl;
        return InputColor();
    }
    return static_cast<Color>(num_color);
}
void InputPlayer(Color & color_player, std::string & name_player) {
    Print("         Enter a name: ", light_blue_two, black);
    std::cin >> name_player;
    Print("      Choose the color of the checkers: ", light_green, black);
    std::cout << std::endl;
    PrintColor();
    color_player = InputColor();
}
Board * Customization() {
    Color first_color_board;
    Color second_color_board;
    Color color_first_player;
    Color color_second_player;
    std::string name_first_player;
    std::string name_second_player;

    std::string message = "CUSTOMIZATION:";
    Print(message, red, black);
    std::cout << std::endl;
    message = "   Choose the color of the playing field: ";
    Print(message, yellow, black);
    std::cout << std::endl;
    message = "      The first color of the board: ";
    Print(message, light_green, black);
    std::cout << std::endl;
    PrintColor();
    first_color_board = InputColor();
    message = "      The second color of the board: ";
    Print(message, light_green, black);
    std::cout << std::endl;
    PrintColor();
    second_color_board = InputColor();
    message = "   First player: ";
    Print(message, yellow, black);
    std::cout << std::endl;
    InputPlayer(color_first_player, name_first_player);
    message = "   Second player: ";
    Print(message, yellow, black);
    std::cout << std::endl;
    InputPlayer(color_second_player, name_second_player);
    std::cout << std::endl;
    Print("          START GAME          " , light_blue_three, light_gray);
    std::cout << std::endl;


    Player * player_1 = new Player(name_first_player, color_first_player);
    Player * player_2 = new Player(name_second_player, color_second_player);
    Board * board = new Board(first_color_board, second_color_board, *player_1, *player_2);
    return board;
}
Game::Game() {
    Hello();
    this->board = Customization();
}
HowMove Game::GetMove() {
    std::string from_str;
    std::string in_str;
    std::string message;
    Print("MOVE: ", light_green, black);
    Print("   ", black, (*board).GetMovePlayer().GetColor()
    );
    std::cout << " - " << (*board).GetMovePlayer().GetName() << std::endl;
    Print("   From: ", light_blue_one, black);
    std::cin >> from_str;
    if (from_str[0] == '\\') {
        Cheats(from_str, board);
        UpdateField();
        return can_move_again;
    }
    Print("   In: ", light_blue_one, black);
    std::cin >> in_str;
    std::pair<int, int> from(from_str[1] - '1', 'h' - from_str[0] );
    std::pair<int, int> in(in_str[1] - '1' , 'h' - in_str[0]);
    return (*board).Move(from, in);
}
void Game::PrintBoard() {
    std::cout << std::endl;
    int n = 0;
    int num = 9;
    for (int i = 8; i > -2; i--) {
        for (int j = 8; j > -2; j--) {
            if (i == -1 || i == 8) {
                if (j > -1 && j < 8)
                    std::cout << " " << char('h' - j) << " ";
                else
                    Print("   ", black, black);
            } else if (i != -1 && i != 8 && (j == -1 || j == 8)) {
                std::cout << " " << num << " ";
            } else {
                std::pair<int, int> pos (i, j);
                Color color_board_cell;
                if (n % 2 == 0) {
                    color_board_cell = (*board).GetFirstColor();
                } else {
                    color_board_cell = (*board).GetSecondColor();
                }
                if ((*board).FindBoard(pos) && (*board).GetBoard().at(pos) != nullptr) {
                    std::string checker;
                    if ((*board).GetBoard().at(pos)->GetKing())
                        checker = ' ' + (*board).GetBoard().at(pos)->GetSymKing() + ' ';
//                        checker = ' ' + board.at(pos)->GetSymKing() + ' ';
                    else
                        checker = ' ' + (*board).GetBoard().at(pos)->GetSymChecker() + ' ';
//                        checker = ' ' + board.at(pos)->GetSymChecker() + ' ';
                    Print(checker, (*board).GetBoard().at(pos)->GetColor(), color_board_cell);
                } else {
                    Print("   ", black, color_board_cell);
                }
                n++;
            }
        }
        num--;
        n++;
        std::cout << std::endl;
    }
    std::cout << std::endl;
//    board->PrintBoard();
}
void Game::UpdateField() {
    (*board).CheckBoard();
}
void Game::Winner() {
    std::cout << std::endl;
    Print("          GAME OVER!          ", light_blue_three, light_gray);
    std::cout << "\n" << std::endl;
    Print("   The winner is ", green, black);
    Print(GetBoard()->GetMovePlayer().GetName(), GetBoard()->GetMovePlayer().GetColor(), black);
    Print("!", green, black);
    std::cout << std::endl;
}