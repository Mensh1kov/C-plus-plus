#pragma once
#include <string>
#include <set>

class Checker;

enum Color {
    black = 0,
    blue,
    green,
    light_blue_one,
    red,
    magenta,
    brown,
    light_gray,
    dark_gray,
    light_blue_two,
    light_green,
    light_blue_three,
    light_red,
    light_purple,
    yellow,
    white
};

struct Player {
private:
    const std::string name;
    const Color color;
    bool can_fell = false;
    std::set<Checker *> checkers = {};
    int count_checkers = 12;
public:
    Player(std::string name, Color color) : name(std::move(name)), color(color) {};

    Color GetColor() const {return color;}
    std::string GetName() const {return name;}
    int GetCountCheckers() const {return count_checkers;}
    bool GetCanFell() const {return can_fell;}
    bool FindChecker(Checker * checker) const {return checkers.find(checker) != checkers.end();}
    const std::set<Checker *> & GetCheckers() const {return checkers;}

    void InsetrtChecker(Checker * checker) {this->checkers.insert(checker);}
    void DeleteChecker(Checker * del_checker) {checkers.erase(checkers.find(del_checker)); count_checkers--;}
    void EditCanFell(bool edit) {this->can_fell = edit;}
};