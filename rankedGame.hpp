#ifndef RANKEDGAME_HPP
#define RANKEDGAME_HPP
#include <iostream>
#include "games.hpp"
using namespace std;


class Ranked : private Game
{
public:
    Ranked(string inviter, string invited, int invitation_id) : Game(inviter, invited, invitation_id){

    };
private:
    int turn_number_ = 1;
};

#endif