#ifndef GAMES_HPP
#define GAMES_HPP

#include <iostream>
using namespace std;


class Game
{
public:
    Game(string inviter, string invited);
private:
    string inviter_;
    string invited_;
    bool isFinished = false;
    string winner = "";
    string loser = "";
};

#endif