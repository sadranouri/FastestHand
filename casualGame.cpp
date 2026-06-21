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
    if(current_player->getCurrentCasualAct().size() != 0)
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
    winner->endGame();
    loser->endGame();
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
    current_player->performCasualAction(SHOOT);
    other_player->performCasualAction(other_player->getCasualGameStatus().act);

    current_player->addCasualAct(SHOOT);
    current_player->changeCasualAct("");
    other_player->changeCasualAct("");
    increaseTurnNumber();

    if(other_player->getCasualGameStatus().act == SHOOT)
    {
        other_player->addCasualAct(SHOOT);
        return false;
    }
    else if(other_player->getCasualGameStatus().act == DEFEND)
    {
        other_player->addCasualAct(DEFEND);
        return false;
    }
    else if(other_player->getCasualGameStatus().act == RELOAD)
    {
        other_player->addCasualAct(RELOAD);
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
    current_player->performCasualAction(RELOAD);
    other_player->performCasualAction(other_player->getCasualGameStatus().act);

    current_player->addCasualAct(RELOAD);
    current_player->changeCasualAct("");
    other_player->changeCasualAct("");
    increaseTurnNumber();

    if(other_player->getCasualGameStatus().act == RELOAD)
    {
        other_player->addCasualAct(RELOAD);
        return true;
    }
    else if(other_player->getCasualGameStatus().act == DEFEND)
    {
        other_player->addCasualAct(DEFEND);
        return true;
    }
    else if(other_player->getCasualGameStatus().act == SHOOT)
    {
        other_player->addCasualAct(SHOOT);
        decreaseTurnNumber();
        return false;
    }
    return true;
}


void Casual::defend(Player *current_player, Player *other_player)
{
    if(other_player->getCasualGameStatus().act == "")
    {
        current_player->changeCasualAct(DEFEND);
        return;
    }
    other_player->performCasualAction(other_player->getCasualGameStatus().act);
    
    current_player->addCasualAct(DEFEND);
    current_player->changeCasualAct("");
    other_player->changeCasualAct("");
    increaseTurnNumber();
    
    if(other_player->getCasualGameStatus().act == SHOOT)
    {
        other_player->addCasualAct(SHOOT);
    }
    else if(other_player->getCasualGameStatus().act == RELOAD)
    {
        other_player->addCasualAct(RELOAD);
    }
    else if(other_player->getCasualGameStatus().act == DEFEND)
    {
        other_player->addCasualAct(DEFEND);
    }
}