#ifndef PLAYERS_HPP
#define PLAYERS_HPP
#include <iostream>
#include <vector>
#define LOGGED_IN "logged in"
#define LOGGED_OUT "logged out"
using namespace std;


struct CasualGame
{
    int bullets = 1;
    string act = "";
    vector<string> actions;
};



class Player
{
public:
    Player(string username, string password);
    Player(string username, string password, double XP, double RP);
    string getUsername();
    string getPassword();
    string getStatus();
    string getCurrentAct();
    string getRankedLevel();
    double getXP();
    double getRP();
    int getRemainingBullets();
    void changeStatus();
    void changeReadyStatus(bool status);
    void changeMatchType(string match_type);
    void changePlayingStatus(bool playingStatus);
    void startCasualGame();
    void endCasualGame();
    void changeCasualAct(string act);
    void performAction(string act);
    void increaseXP(double XP);
    void decreaseXP(double XP);
    void addAct(string act);
    void rankLeveling(double RP);
    bool getReadyStatus();
    bool getPlayingStatus();
    vector<string> getActions();
    CasualGame getCasualGameStatus();
private:
    string username_;
    string password_;
    double XP_;
    double RP_;
    string status_ = LOGGED_OUT;
    bool casual_match_ready_ = false;
    bool is_playing_ = false;
    string match_type_ = "";
    CasualGame current_match_;
    string ranked_level_;
};

#endif