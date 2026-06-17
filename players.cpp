#include <iostream>
#include "players.hpp"
#define LOGGED_IN "logged in"
#define LOGGED_OUT "logged out"
#define BRONZE "bronze"
#define SILVER "silver"
#define GOLD "gold"
#define PLATINUM "platinum"
using namespace std;


Player::Player(string username, string password)
{
    username_ = username;
    password_ = password;
    XP_ = 500;
    RP_ = 1200;
    rankLeveling(1200);
    status_ = LOGGED_OUT;
    casual_match_ready_ = false;
    is_playing_ = false;
    match_type_ = "";
}


Player::Player(string username, string password, double XP, double RP)
{
    username_ = username;
    password_ = password;
    XP_ = XP;
    RP_ = RP;
    rankLeveling(RP);
    status_ = LOGGED_OUT;
    casual_match_ready_ = false;
    is_playing_ = false;
    match_type_ = "";
}


string Player::getUsername()
{
    return username_;
}


string Player::getPassword()
{
    return password_;
}


string Player::getStatus()
{
    return status_;
}


void Player::changeStatus()
{
    if(status_ == LOGGED_IN)
    {
        status_ = LOGGED_OUT;
    }
    else
    {
        status_ = LOGGED_IN;
    }
}


void Player::changeReadyStatus(bool status)
{
    casual_match_ready_ = status;
}


bool Player::getReadyStatus()
{
    return casual_match_ready_;
}


double Player::getXP()
{
    return XP_;
}


bool Player::getPlayingStatus()
{
    return is_playing_;
}


void Player::changeMatchType(string match_type)
{
    match_type_ = match_type;
}


void Player::changePlayingStatus(bool playingStatus)
{
    is_playing_ = playingStatus;
}


void Player::startCasualGame()
{
    casual_match_ = CasualGame();
}

void Player::endCasualGame()
{
    is_playing_ = false;
}


CasualGame Player::getCasualGameStatus()
{
    return casual_match_;
}


void Player::changeCasualAct(string act)
{
    casual_match_.act = act;
}


void Player::performAction(string act)
{
    if(act == "shoot")
    {
        casual_match_.bullets -= 1;
    }
    else if(act == "reload")
    {
        casual_match_.bullets += 1;
    }
}


void Player::increaseXP(double XP)
{
    XP_ += XP;
}


void Player::decreaseXP(double XP)
{
    XP_ -= XP;
}


void Player::addAct(string act)
{
    casual_match_.actions.push_back(act);
}


string Player::getCurrentAct()
{
    return casual_match_.act;
}


vector<string> Player::getActions()
{
    return casual_match_.actions;
}


int Player::getRemainingBullets()
{
    return casual_match_.bullets;
}


double Player::getRP()
{
    return RP_;
}


void Player::rankLeveling(double RP)
{
    if(RP < 1400)
    {
        ranked_level_ = BRONZE;
    }
    else if(RP >= 1400 && RP < 1750)
    {
        ranked_level_ = SILVER;
    }
    else if(RP >= 1750 && RP < 2250)
    {
        ranked_level_ = GOLD;
    }
    else if(RP >= 2250)
    {
        ranked_level_ = PLATINUM;
    }
}


string Player::getRankedLevel()
{
    return ranked_level_;
}


void Player::blockPlayer(string username)
{
    blocked_players_.push_back(username);
}


vector<string> Player::getBlockedPlayers()
{
    return blocked_players_;
}


void Player::startRankedGame()
{
    ranked_match_ = RankedGame();
}