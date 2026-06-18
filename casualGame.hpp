#ifndef CASUALGAME_HPP
#define CASUALGAME_HPP
#include <iostream>
#include "players.hpp"
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
    int getTurnNumber();
    void performAction(Player *current_player, Player *other_player, string act);
    void finishGame(Player *winner, Player *loser);
    void defend(Player *current_player, Player *other_player);
    bool shoot(Player *current_player, Player *other_player);
    bool reload(Player *current_player, Player *other_player);
private:
    int turn_number_ = 1;
};

#endif