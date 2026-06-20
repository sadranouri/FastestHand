#ifndef RANKEDGAME_HPP
#define RANKEDGAME_HPP
#include <iostream>
#include "games.hpp"
#include "players.hpp"
using namespace std;


class Ranked : public Game
{
public:
    Ranked(string inviter, string invited, int invitation_id) : Game(inviter, invited, invitation_id){

    };
    void increaseTurnNumber();
    void decreaseTurnNumber();
    void performAction(Player *current_player, Player *other_player, string act);
    void finishMatch(Player *winner, Player *loser);
    void defend(Player *current_player, Player *other_player);
    bool shoot(Player *current_player, Player *other_player);
    bool reload(Player *current_player, Player *other_player);
    double matchRP(Player winner);
    double healthBonus(Player winner);
    int getTurnNumber();
private:
    int turn_number_ = 1;
};

#endif