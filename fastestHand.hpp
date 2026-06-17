#pragma once
#include <iostream>
#include <vector>
#include "players.hpp"
#include "admins.hpp"
#include "games.hpp"
#include "casualGame.hpp"
#include "rankedGame.hpp"
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
    int id;
    string match_type;
    bool isAccepted = false;
    bool isRejected = false;
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
    void casualPerformAction(Invitation* invite, string act);
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
    void outputRankedPlayers(vector<Player> ranked_players);
    void inviteCreator(string match_type, string invited);
    void block();
    void endCasualGame(Invitation *match, Player *winner, Player *loser);
    void startCasualMatch(Player *player1, Player *player2);
    void startRankedMatch(Player *player1, Player *player2);
    void rankedPerformAction(Invitation *invite, string act);
    void rankedShoot(Invitation *invite, Player *current_player, Player *other_player);
    void rankedReload(Invitation *invite, Player *current_player, Player *other_player);
    void rankedDefend(Invitation *invite, Player *current_player, Player *other_player);
    bool casualShoot(Invitation* invite, Player* current_player, Player* other_player);
    bool casualReload(Invitation* invite, Player* current_player, Player* other_player);
    bool usernameAlreadyExists(string username);
    bool wrongPassword(string username, string password);
    bool usernameNotFound(string username);
    bool adminUsername(string username);
    bool invitedPlayerBlockedYou(string invited);
    int totalWins(string username);
    int totalLosses(string username);
    double casualXP(Player winner, Player loser);
    map<string, string> FastestHand::parseArguments();
    vector<Player> inOrderReadyPlayers(string order_type);
    vector<Player> inOrderRankedPlayers(string order_type);
    vector<Invitation> inOrderInvitations(string username);
    vector<Report> inOrderReports();
    vector<Player>::iterator findPlayerByUsername(string username);
    vector<Admin>::iterator findAdminByUsername(string username);
    vector<Player> players;
    vector<Admin> admins;
    vector<Invitation> invitations;
    vector<Casual> casual_matches;
    vector<Ranked> ranked_matches;
    vector<Report> reports;
    Session session;
    int invitation_id_;
    int report_id_;
};