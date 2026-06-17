#ifndef GAMES_HPP
#define GAMES_HPP
#include <iostream>
using namespace std;


class Game
{
public:
    Game(string inviter, string invited, int invitation_id){
        inviter_ = inviter;
        invited_ = invited;
        invitation_id_ = invitation_id;
    }
    string getInviter();
    string getInvited();
    string getWinner();
    string getLoser();
    void finishGame();
    void matchOutcome(string winner, string loser);
    bool isFinished();
private:
    string inviter_;
    string invited_;
    int invitation_id_;
    bool isFinished_ = false;
    string winner_ = "";
    string loser_ = "";
};

#endif