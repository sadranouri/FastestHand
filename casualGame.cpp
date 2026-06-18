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
            finishGame(&(*current_player), &(*other_player));
        }
    }
    else if(act == RELOAD)
    {
        if(!reload(&(*current_player), &(*other_player)))
        {
            finishGame(&(*other_player), &(*current_player));
        }
    }
    else if(act == DEFEND)
    {
        defend(&(*current_player), &(*other_player));
    }
    
    
    cout << OK << endl;
}