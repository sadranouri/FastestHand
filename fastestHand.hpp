#pragma once
#include <iostream>
#include <vector>
#include <map>
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
private:
    void post(string command);
    void get(string command);
    void readPlayersCSV(string playersFile);
    void readAdminsCSV(string adminsFile);
    void answerRequest();
    void playerRegister(map<string, string> setArgs);
    void login(map<string, string> setArgs);
    void logout();
    void casualMatchReady(map<string, string> setArgs);
    void casualMatchOpponents(map<string, string> setArgs);
    void invitation(map<string, string> setArgs);
    void startMatch(map<string, string> setArgs);
    void rejectInvitation(map<string, string> setArgs);
    void action(map<string, string> setArgs);
    void matchStatus();
    void casualMatchStatusOutput(Player current_player, Player other_player, Casual match);
    void rankedMatchStatusOutput(Player current_player, Player other_player, Ranked match);
    void report(map<string, string> setArgs);
    void profile(map<string, string> setArgs);
    void myProfile();
    void othersProfile(string username);
    void receivedInvitations();
    void reportsOutput();
    void rankedMatchOpponents(map<string, string> setArgs);
    void outputRankedPlayers(vector<Player> ranked_players);
    void inviteCreator(string match_type, string invited);
    void block(map<string, string> setArgs);
    void startCasualMatch(Player *inviter, Player *invited, int invitation_id);
    void startRankedMatch(Player *inviter, Player *invited, int invitation_id);
    void penalty(map<string, string> setArgs);
    void dismissReport(map<string, string> setArgs);
    bool usernameAlreadyExists(string username);
    bool wrongPassword(string username, string password);
    bool usernameNotFound(string username);
    bool adminUsername(string username);
    bool invitedPlayerBlockedYou(string invited);
    bool outOfRangeAmount(string type, int amount);
    int totalWins(string username);
    int totalLosses(string username);
    map<string, string> parseArguments();
    vector<Player> inOrderReadyPlayers(string order_type);
    vector<Player> inOrderRankedPlayers(string order_type);
    vector<Invitation> inOrderInvitations(string username);
    vector<Report> inOrderReports();
    vector<Player>::iterator findPlayerByUsername(string username);
    vector<Admin>::iterator findAdminByUsername(string username);
    vector<Player>::iterator findOtherPlayerForMatchStatus(vector<Ranked>::iterator ranked_it, vector<Casual>::iterator casual_it, vector<Player>::iterator current_player);
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