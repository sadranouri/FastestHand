#pragma once
#include <iostream>
#include <vector>
#include "players.hpp"
#include "admins.hpp"
using namespace std;


struct Session
{
    bool isPlayer = false;
    bool isAdmin = false;
    string username;
};


struct Invitation
{
    string inviter;
    string invited;
    string match_type;
    int id;
    int turn_number = 1;
    bool isAccepted = false;
    bool isRejected = false;
    bool isFinished = false;
    string winner = "";
    string loser = "";
};


struct Report
{
    string reporter;
    string reported;
    string reason;
    int id;
};


class FastestHand
{
public:
    FastestHand(string playersFile, string adminsFile);
    void run(string playersFile, string adminsFile);
public:
    void post(string command);
    void get(string command);
    void readPlayersCSV(string playersFile);
    void readAdminsCSV(string adminsFile);
    void answerRequest();
    void playerRegister();
    void login();
    void logout();
    void casualMatchReady();
    void casualMatchOpponents();
    void invitation();
    void startMatch();
    void rejectInvitation();
    void action();
    void performAction(Invitation* invite, string act);
    void casualDefend(Invitation* invite, Player* current_player, Player* other_player);
    void matchStatus();
    void matchStatusOutput(Player current_player, Player other_player, Invitation match);
    void report();
    void profile();
    void myProfile();
    void othersProfile(string username);
    void receivedInvitations();
    void reportsOutput();
    void rankedMatchOpponents();
    void inOrderRankedPlayers(string order_type);
    bool casualShoot(Invitation* invite, Player* current_player, Player* other_player);
    bool casualReload(Invitation* invite, Player* current_player, Player* other_player);
    bool usernameAlreadyExists(string username);
    bool wrongPassword(string username, string password);
    int totalWins(string username);
    int totalLosses(string username);
    double casualXP(Player winner, Player loser);
    vector<Player> inOrderReadyPlayers(string order_type);
    vector<Invitation> inOrderInvitations(string username);
    vector<Report> inOrderReports();
    vector<Player> players;
    vector<Admin> admins;
    vector<Invitation> invitations;
    vector<Report> reports;
    Session session;
    int invitation_id_;
    int report_id_;
};