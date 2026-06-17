#ifndef CASUALGAME_HPP
#define CASUALGAME_HPP
#include <iostream>
#include "games.hpp"
using namespace std;


class Casual : private Game
{
public:
    Casual(string inviter, string invited, int invitation_id) : Game(inviter, invited, invitation_id)
    {

    };
private:
    int turn_number = 1;
};

#endif