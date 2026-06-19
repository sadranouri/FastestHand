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


struct RankedGame
{
    int bullets = 3;
    int health = 3;
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
    string getCurrentCasualAct();
    string getCurrentRankedAct();
    string getRankedLevel();
    double getXP();
    double getRP();
    int getRemainingCasualBullets();
    int getCurrentRankedHealth();
    void changeStatus();
    void changeReadyStatus(bool status);
    void changeMatchType(string match_type);
    void changePlayingStatus(bool playingStatus);
    void startCasualGame();
    void endGame();
    void changeCasualAct(string act);
    void changeRankedAct(string act);
    void performCasualAction(string act);
    void performRankedAction(string act);
    void increaseXP(double XP);
    void decreaseXP(double XP);
    void increaseRP(double RP, double health_bonus);
    void decreaseRP(double RP);
    void addCasualAct(string act);
    void addRankedAct(string act);
    void rankLeveling(double RP);
    void blockPlayer(string username);
    void startRankedGame();
    void decreaseRankedHealth();
    bool getReadyStatus();
    bool getPlayingStatus();
    vector<string> getActions();
    vector<string> getBlockedPlayers();
    CasualGame getCasualGameStatus();
    RankedGame getRankedGameStatus();
private:
    string username_;
    string password_;
    double XP_;
    double RP_;
    string status_ = LOGGED_IN;
    bool casual_match_ready_ = false;
    bool is_playing_ = false;
    string match_type_ = "";
    CasualGame casual_match_;
    RankedGame ranked_match_;
    string ranked_level_;
    vector<string> blocked_players_;
};

#endif