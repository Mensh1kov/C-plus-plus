#include "board.h"


const std::pair<int, int> NULL_CELL(NULL, NULL);

enum HowMove CheckErrors(const std::pair<int, int> & from, const std::pair<int, int> & in, Board * board) {
    if (!(board->GetMovePlayer().FindChecker(board->GetBoard()[from]))) {
        return from_cell_err;
    } else if (board->GetMovePlayer().GetCanFell() != board->GetBoard()[from]->GetCanFell()) {
        return fell_err;
    } else if (!(board->GetBoard()[from]->FindCanMove(in))) {
        return in_cell_err;
    } else {
        return successfully;
    }
}
void CheckFellForSimpleChecker(std::pair<int, int> opponent, std::pair<int, int> can_move, Checker * checker, Board * board) {
    if (board->FindBoard(opponent) && board->FindBoard(can_move)) { // была сделана поправка
        Checker * suspect_checker = board->GetBoard().at(opponent);
        if (board->GetSleepPlayer().FindChecker(suspect_checker)) {
            if (board->GetBoard().at(can_move) == nullptr) {
                checker->GetCanMove()[can_move] = opponent;
                checker->EditCanFell(true);
                board->GetMovePlayer().EditCanFell(true);
            }
        }
    }
}
void GeneratingBoard(Board * this_board, const Color& color_first_player, const Color& color_second_player) {
    int n = 0;
    int m = 0;
    for (int i = 0; i < 8; i++) {
        int shift = (i % 2 == 0) ? shift = 1 : shift = 0;
        for (int j = shift; j < 8; j += 2) {
            Checker *new_checker = nullptr;
            if (i < 3) {
                new_checker = new Checker(std::pair<int, int>(i, j), color_first_player, first_player, 'o', '*');
                this_board->GetMovePlayer().InsetrtChecker(new_checker);
            } else if (i > 4) {
                new_checker = new Checker(std::pair<int, int>(i, j), color_second_player, second_player, 'o', '*');
                this_board->GetSleepPlayer().InsetrtChecker(new_checker);
            }
            this_board->GetBoard()[std::pair<const int, const int>(i, j)] = new_checker;
        }
    }
}
void CheckCanFellSimpleChecker(Board * board, Checker * checker) {
    const std::set<int> set = {1, -1};
    for (auto i : set) {
        for (auto j : set) {
            int x = i + checker->GetPosition().first;
            int y = j + checker->GetPosition().second;
            std::pair<int, int> opponent(x, y);
            x += i;
            y += j;
            std::pair<int, int> can_move(x, y);
            CheckFellForSimpleChecker(opponent, can_move, checker, board);
        }
    }
}
void CheckCanMoveSimpleChecker(Board * board, Checker * checker) {
    std::pair<int, int> right = checker->RightMove();
    std::pair<int, int> left = checker->LeftMove();
    if (board->FindBoard(right) && board->GetBoard()[right] == nullptr)
        checker->GetCanMove()[right] = NULL_CELL;
    if (board->FindBoard(left) && board->GetBoard()[left] == nullptr)
        checker->GetCanMove()[left] = NULL_CELL;
}
Board::Board(Color first_color, Color second_color, Player& first_player, Player& second_player) {
    this->first_color = first_color;
    this->second_color = second_color;
    this->move_player = &first_player;
    this->sleep_player = &second_player;
    GeneratingBoard(this, first_player.GetColor(), second_player.GetColor());
}
void OffCanFellAllCheckers(Board * board, const std::pair<int, int> & exception) {
    for (auto checker : board->GetMovePlayer().GetCheckers()) {
        if (checker->GetPosition() != exception)
            checker->EditCanFell(false);
    }
}
void SumPairInt(std::pair<int, int> & edit_this, const std::pair<int, int> & shift) {
    edit_this.first += shift.first;
    edit_this.second += shift.second;
}
bool CheckCell(std::pair<int, int> cell, const std::pair<int, int> shift, const std::pair<int, int> & pos_felled_checker, std::map<std::pair<int, int>, std::pair<int,int>> & can_fell, Board * board) {
    std::pair<int, int> start_cell = cell;
    std::pair<int, int> shift_1 = shift;
    std::pair<int, int> shift_2 = shift;
    shift_1.first *= -1;
    shift_2.second *= -1;
    const std::set<std::pair<int, int>> set = {shift_1, shift_2};
    for (auto shift_n : set) {
        cell = start_cell;
        while (board->FindBoard(cell)) {
            if (board->GetBoard()[cell] == nullptr) {
                SumPairInt(cell, shift_n);
            } else if (board->GetSleepPlayer().FindChecker(board->GetBoard()[cell])) {
                SumPairInt(cell, shift_n);
                if (board->FindBoard(cell) && board->GetBoard()[cell] == nullptr) {
//                can_fell[start_cell] = pos_felled_checker;
                    return true;
                } else
                    break;
            } else
                break;
        }
    }
    return false;
}
void CheckDiagonal(const std::pair<int, int>& shift, std::map<std::pair<int, int>, std::pair<int, int>> & can_move, std::map<std::pair<int, int>, std::pair<int, int>> & can_fell, Board * board, Checker * checker) {
    std::pair<int, int> pos_felled_checker;
    std::pair<int, int> cell = checker->GetPosition();
    std::set<std::pair<int, int>> set_pos;
    SumPairInt(cell, shift);

    while (board->FindBoard(cell)) {
        if (board->GetBoard()[cell] == nullptr) {
            if (!(checker->GetCanFell()))
                can_move[cell] = NULL_CELL;
            SumPairInt(cell, shift);
        } else if (board->GetSleepPlayer().FindChecker(board->GetBoard()[cell])) {
            pos_felled_checker = cell;
            SumPairInt(cell, shift);
            if (board->FindBoard(cell) && board->GetBoard()[cell] == nullptr) {
                checker->EditCanFell(true);
                while (board->FindBoard(cell) && board->GetBoard()[cell] == nullptr) {
                    set_pos.insert(cell);
                    SumPairInt(cell, shift);
                }
                if (board->FindBoard(cell) && board->GetSleepPlayer().FindChecker(board->GetBoard()[cell])) {
                    SumPairInt(cell, shift);
                    if (board->FindBoard(cell) && board->GetBoard()[cell] == nullptr) {
                        for (auto i: set_pos)
                            can_fell[i] = pos_felled_checker;
                        return;
                    }
                }
                bool check_cell = true; // если после первой срубленной шашки больше возможности рубить нет, то будет true;
                for (auto j : set_pos) {
                    if (CheckCell(j, shift, pos_felled_checker, can_fell, board)) {
                        check_cell = false;
                        can_fell[j] = pos_felled_checker;
                    }
                }
                if (check_cell) {
                    for (auto i : set_pos)
                        can_fell[i] = pos_felled_checker;
                }
                return;
            } else {
                return;
            }
        } else {
            return;
        }
    }
}
void CheckMoveSimpleChecker(Board * board, Checker * checker) {
    CheckCanFellSimpleChecker(board, checker);
    if (!(checker->GetCanFell()))
        CheckCanMoveSimpleChecker(board, checker);
}
void CheckMoveKingChecker(Board * board, Checker * checker) {
    const std::set<int> set = {1, -1};
    std::map<std::pair<int, int>, std::pair<int, int>> can_move;
    std::map<std::pair<int, int>, std::pair<int, int>> can_fell;
    for (auto i : set) {
        for (auto j : set) {
            std::pair<int, int> shift(i, j);
            CheckDiagonal(shift, can_move, can_fell, board, checker);
        }
    }
    if (checker->GetCanFell()) {
        board->GetMovePlayer().EditCanFell(true);
        checker->GetCanMove() = can_fell;
    } else {
        checker->GetCanMove() = can_move;
    }
}
void CheckMove(Board * board, Checker * checker) {
    checker->GetCanMove() = {};
    checker->EditCanFell(false);
    if (checker->GetKing()){
        CheckMoveKingChecker(board, checker);
    } else {
        CheckMoveSimpleChecker(board, checker);
    }
}
HowMove Board::Move(std::pair<int, int> from, std::pair<int, int> in) {
    Checker * moved_checker;
    std::pair<int, int> pos_felled_checker;
    enum HowMove error = CheckErrors(from, in, this);
    if (error == successfully) {
        std::swap(GetBoard()[from], GetBoard()[in]);
        moved_checker = GetBoard()[in];
        if (in.first == 0 && moved_checker->GetOrientation() == -1 || in.first == 7 && moved_checker->GetOrientation() == 1)
            moved_checker->EditKing(true);
        moved_checker->EditPosition(in);
        GetMovePlayer().EditCanFell(false);
        if (moved_checker->GetCanFell()) {
            pos_felled_checker = moved_checker->GetCanMove()[in];
            GetSleepPlayer().DeleteChecker(GetBoard()[pos_felled_checker]);
            delete GetBoard()[pos_felled_checker];
            GetBoard()[pos_felled_checker] = nullptr;
            if (GetSleepPlayer().GetCountCheckers() == 0)
                return winner;
            CheckMove(this, moved_checker);
            if (GetMovePlayer().GetCanFell()) {
                OffCanFellAllCheckers(this, in);
                return can_move_again;
            }
        }
        EditMovePlayer();
        return move_transition;
    } else {
        return error;
    }
}
void Board::CheckBoard() {
    for (auto checker : GetMovePlayer().GetCheckers()) {
        CheckMove(this, checker);
    }
}
