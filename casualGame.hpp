#ifndef CASUALGAME_HPP
#define CASUALGAME_HPP
#include <iostream>
#include "games.hpp"
using namespace std;


class Casual : public Game
{
public:
    Casual(string inviter, string invited, int invitation_id) : Game(inviter, invited, invitation_id)
    {

    };
    void increaseTurnNumber();
    void decreaseTurnNumber();
private:
    int turn_number_ = 1;
};

#endif