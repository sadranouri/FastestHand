#include <iostream>
#include "casualGame.hpp"
#define OK "OK"
#define PERMISSION_DENIED "Permission Denied"
#define BAD_REQUEST "Bad Request"
#define EMPTY "Empty"
#define NOT_FOUND "Not Found"
#define SHOOT "shoot"
#define DEFEND "defend"
#define RELOAD "reload"
using namespace std;


void Casual::increaseTurnNumber()
{
    turn_number_++;
}


void Casual::decreaseTurnNumber()
{
    turn_number_--;
}


int Casual::getTurnNumber()
{
    return turn_number_;
}


void Casual::performAction(Player *current_player, Player *other_player, string act)
{
    if(current_player->getCurrentAct().size() != 0)
    {
        cout << PERMISSION_DENIED << endl;
        return;
    }

    if(act == SHOOT)
    {
        if(current_player->getCasualGameStatus().bullets == 0)
        {
            cout << BAD_REQUEST << endl;
            return;
        }
        if(shoot(&(*current_player), &(*other_player)))
        {
            finishMatch(&(*current_player), &(*other_player));
        }
    }
    else if(act == RELOAD)
    {
        if(!reload(&(*current_player), &(*other_player)))
        {
            finishMatch(&(*other_player), &(*current_player));
        }
    }
    else if(act == DEFEND)
    {
        defend(&(*current_player), &(*other_player));
    }
    
    
    cout << OK << endl;
}


void Casual::finishMatch(Player *winner, Player *loser)
{
    finishGame();
    matchOutcome(winner->getUsername(), loser->getUsername());
    double match_xp = matchXP(*winner, *loser);
    winner->increaseXP(match_xp);
    loser->decreaseXP(match_xp);
    winner->endCasualGame();
    loser->endCasualGame();
}


double Casual::matchXP(Player winner, Player loser)
{
    double xp_difference = 50 - (0.1 * (winner.getXP() - loser.getXP()));

    return (5 > xp_difference) ? 5 : xp_difference;
}


bool Casual::shoot(Player *current_player, Player *other_player)
{
    if(other_player->getCasualGameStatus().act == "")
    {
        current_player->changeCasualAct(SHOOT);
        return false;
    }
    current_player->performAction(SHOOT);
    other_player->performAction(other_player->getCasualGameStatus().act);

    current_player->addAct(SHOOT);
    current_player->changeCasualAct("");
    other_player->changeCasualAct("");
    increaseTurnNumber();

    if(other_player->getCasualGameStatus().act == SHOOT)
    {
        other_player->addAct(SHOOT);
        return false;
    }
    else if(other_player->getCasualGameStatus().act == DEFEND)
    {
        other_player->addAct(DEFEND);
        return false;
    }
    else if(other_player->getCasualGameStatus().act == RELOAD)
    {
        other_player->addAct(RELOAD);
        decreaseTurnNumber();
        return true;
    }
    return false;
}


bool Casual::reload(Player *current_player, Player *other_player)
{
    if(other_player->getCasualGameStatus().act == "")
    {
        current_player->changeCasualAct(RELOAD);
        return true;
    }
    current_player->performAction(RELOAD);
    other_player->performAction(other_player->getCasualGameStatus().act);

    current_player->addAct(RELOAD);
    current_player->changeCasualAct("");
    other_player->changeCasualAct("");
    increaseTurnNumber();

    if(other_player->getCasualGameStatus().act == RELOAD)
    {
        other_player->addAct(RELOAD);
        return true;
    }
    else if(other_player->getCasualGameStatus().act == DEFEND)
    {
        other_player->addAct(DEFEND);
        return true;
    }
    else if(other_player->getCasualGameStatus().act == SHOOT)
    {
        other_player->addAct(SHOOT);
        decreaseTurnNumber();
        return false;
    }
    return false;
}


void Casual::defend(Player *current_player, Player *other_player)
{
    if(other_player->getCasualGameStatus().act == "")
    {
        current_player->changeCasualAct(DEFEND);
    }
    other_player->performAction(other_player->getCasualGameStatus().act);
    
    current_player->addAct(DEFEND);
    current_player->changeCasualAct("");
    other_player->changeCasualAct("");
    increaseTurnNumber();
    
    if(other_player->getCasualGameStatus().act == SHOOT)
    {
        other_player->addAct(SHOOT);
    }
    else if(other_player->getCasualGameStatus().act == RELOAD)
    {
        other_player->addAct(RELOAD);
    }
    else if(other_player->getCasualGameStatus().act == DEFEND)
    {
        other_player->addAct(DEFEND);
    }
}