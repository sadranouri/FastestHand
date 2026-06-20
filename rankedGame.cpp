#include <iostream>
#include "rankedGame.hpp"
#include "players.hpp"
#define OK "OK"
#define PERMISSION_DENIED "Permission Denied"
#define BAD_REQUEST "Bad Request"
#define EMPTY "Empty"
#define NOT_FOUND "Not Found"
#define SHOOT "shoot"
#define DEFEND "defend"
#define RELOAD "reload"
#define BRONZE "bronze"
#define SILVER "silver"
#define GOLD "gold"
#define PLATINUM "platinum"
using namespace std;


void Ranked::increaseTurnNumber()
{
    turn_number_++;
}


void Ranked::decreaseTurnNumber()
{
    turn_number_--;
}


void Ranked::performAction(Player *current_player, Player *other_player, string act)
{
    if(current_player->getCurrentRankedAct().size() != 0)
    {
        cout << PERMISSION_DENIED << endl;
        return;
    }

    if(act == SHOOT)
    {
        if(current_player->getRankedGameStatus().bullets == 0)
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


void Ranked::finishMatch(Player *winner, Player *loser)
{
    finishGame();
    matchOutcome(winner->getUsername(), loser->getUsername());
    double match_rp = matchRP(*winner);
    winner->increaseRP(matchRP(*winner), healthBonus(*winner));
    loser->decreaseRP(matchRP(*winner));
    winner->endGame();
    loser->endGame();
}


double Ranked::matchRP(Player winner)
{
    if(winner.getRankedLevel() == BRONZE) {return 75;}
    if(winner.getRankedLevel() == SILVER) {return 100;}
    if(winner.getRankedLevel() == GOLD) {return 125;}
    if(winner.getRankedLevel() == PLATINUM) {return 150;}
}


double Ranked::healthBonus(Player winner)
{
    return (double)(winner.getCurrentRankedHealth() * 25);
}


bool Ranked::shoot(Player *current_player, Player *other_player)
{
    if(other_player->getRankedGameStatus().act == "")
    {
        current_player->changeRankedAct(SHOOT);
        return false;
    }
    current_player->performRankedAction(SHOOT);
    other_player->performRankedAction(other_player->getRankedGameStatus().act);

    current_player->addRankedAct(SHOOT);
    current_player->changeRankedAct("");
    other_player->changeRankedAct("");
    increaseTurnNumber();

    if(other_player->getRankedGameStatus().act == SHOOT)
    {
        other_player->addRankedAct(SHOOT);
        return false;
    }
    else if(other_player->getRankedGameStatus().act == DEFEND)
    {
        other_player->addRankedAct(DEFEND);
        return false;
    }
    else if(other_player->getRankedGameStatus().act == RELOAD)
    {
        other_player->addRankedAct(RELOAD);
        other_player->decreaseRankedHealth();
        if(other_player->getCurrentRankedHealth() == 0)
        {
            decreaseTurnNumber();
            return true;
        }
        return false;
    }
    return false;
}


bool Ranked::reload(Player *current_player, Player *other_player)
{
    if(other_player->getRankedGameStatus().act == "")
    {
        current_player->changeRankedAct(RELOAD);
        return true;
    }
    current_player->performRankedAction(RELOAD);
    other_player->performRankedAction(other_player->getRankedGameStatus().act);

    current_player->addRankedAct(RELOAD);
    current_player->changeRankedAct("");
    other_player->changeRankedAct("");
    increaseTurnNumber();

    if(other_player->getRankedGameStatus().act == RELOAD)
    {
        other_player->addRankedAct(RELOAD);
        return true;
    }
    else if(other_player->getRankedGameStatus().act == DEFEND)
    {
        other_player->addRankedAct(DEFEND);
        return true;
    }
    else if(other_player->getRankedGameStatus().act == SHOOT)
    {
        other_player->addRankedAct(SHOOT);
        current_player->decreaseRankedHealth();
        if(current_player->getCurrentRankedHealth() == 0)
        {
            decreaseTurnNumber();
            return false;
        }
        return true;
    }
    return true;
}


void Ranked::defend(Player *current_player, Player *other_player)
{
    if(other_player->getRankedGameStatus().act == "")
    {
        current_player->changeRankedAct(DEFEND);
    }
    other_player->performRankedAction(other_player->getRankedGameStatus().act);
    
    current_player->addRankedAct(DEFEND);
    current_player->changeRankedAct("");
    other_player->changeRankedAct("");
    increaseTurnNumber();
    
    if(other_player->getRankedGameStatus().act == SHOOT)
    {
        other_player->addRankedAct(SHOOT);
    }
    else if(other_player->getRankedGameStatus().act == RELOAD)
    {
        other_player->addRankedAct(RELOAD);
    }
    else if(other_player->getRankedGameStatus().act == DEFEND)
    {
        other_player->addRankedAct(DEFEND);
    }
}


int Ranked::getTurnNumber()
{
    return turn_number_;
}