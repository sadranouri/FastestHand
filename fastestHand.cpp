#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <map>
#include <iomanip>
#include <typeinfo>
#include "fastestHand.hpp"
#include "players.hpp"
#include "games.hpp"
#include "casualGame.hpp"
#include "rankedGame.hpp"
#define POST "POST"
#define GET "GET"
#define PUT "PUT"
#define DELETE "DELETE"
#define OK "OK"
#define PERMISSION_DENIED "Permission Denied"
#define BAD_REQUEST "Bad Request"
#define EMPTY "Empty"
#define NOT_FOUND "Not Found"
#define LOGGED_IN "logged in"
#define LOGGED_OUT "logged out"
#define LEVEL_MISMATCH "Level Mismatch"
#define SHOOT "shoot"
#define DEFEND "defend"
#define RELOAD "reload"
#define BRONZE "Bronze"
#define SILVER "Silver"
#define GOLD "Golden"
#define PLATINUM "Platinum"
#define CASUAL "casual"
#define RANKED "ranked"
#define BLOCKED "blocked"
#define UNBLOCKED "unblocked"
using namespace std;


FastestHand::FastestHand(string playersFile, string adminsFile)
{
    invitation_id_ = 1;
    report_id_ = 1;
    run(playersFile, adminsFile);
}


void FastestHand::run(string playersFile, string adminsFile)
{
    readPlayersCSV(playersFile);
    readAdminsCSV(adminsFile);
    answerRequest();
}


void FastestHand::readPlayersCSV(string playersFile)
{
    ifstream file(playersFile);
    string line;

    getline(file, line);

    while(getline(file, line))
    {
        istringstream ss(line);
        string username;
        string password;
        string strXP;
        string strRP;

        if(getline(ss, username, ','))
        {
            if(getline(ss, password, ','))
            {
                if(getline(ss, strXP, ','))
                {
                    double XP = stod(strXP);
                    if(getline(ss, strRP))
                    {
                        double RP = stod(strRP);
                        Player newPlayer(username, password, XP, RP);
                        players.push_back(newPlayer);
                    }
                }
            }
        }
    }
}


void FastestHand::readAdminsCSV(string adminsFile)
{
    ifstream file(adminsFile);
    string line;

    getline(file, line);

    while(getline(file, line))
    {
        istringstream ss(line);
        string username;
        string password;

        if(getline(ss, username, ','))
        {
            if(getline(ss, password))
            {
                Admin newAdmin(username, password);
                admins.push_back(newAdmin);
            }
        }
    }
}


void FastestHand::answerRequest()
{
    string request;
    
    while(cin >> request)
    {
        if(request == POST)
        {
            string command;
            string question_mark;
            cin >> command >> question_mark;
            if(question_mark != "?")
            {
                cout << BAD_REQUEST << endl;
                string dummy;
                getline(cin, dummy);
                continue;
            }

            post(command);
        }
        else if(request == GET)
        {
            string command;
            string question_mark;
            cin >> command >> question_mark;
            if(question_mark != "?")
            {
                cout << BAD_REQUEST << endl;
                string dummy;
                getline(cin, dummy);
                continue;
            }

            get(command);
        }
        else if(request == PUT)
        {

        }
        else if(request == DELETE)
        {

        }
        else
        {
            cout << BAD_REQUEST << endl;
            string dummy;
            getline(cin, dummy);
            continue;
        }
    }
}


void FastestHand::post(string command)
{
    map<string, string> setArgs = parseArguments();

    if(command == "register")
    {
        playerRegister(setArgs);
    }
    else if(command == "login")
    {
        login(setArgs);
    }
    else if(command == "logout")
    {
        logout();
    }
    else if(command == "casual_match_ready")
    {
        casualMatchReady(setArgs);
    }
    else if(command == "invitation")
    {
        invitation(setArgs);
    } 
    else if(command == "start_match")
    {
        startMatch(setArgs);
    }
    else if(command == "reject_invitation")
    {
        rejectInvitation(setArgs);
    }
    else if(command == "action")
    {
        action(setArgs);
    }
    else if(command == "report")
    {
        report(setArgs);
    }
    else if(command == "penalty")
    {
        penalty(setArgs);
    }
    else if(command == "dismiss_report")
    {
        dismissReport(setArgs);
    }
    else if(command == "block")
    {
        block(setArgs);
    }
    else
    {
        cout << NOT_FOUND << endl;
    }
}


void FastestHand::get(string command)
{
    map<string, string> setArgs = parseArguments();

    if(command == "match_status")
    {
        matchStatus();
    }
    else if(command == "profile")
    {
        profile(setArgs);
    }
    else if(command == "received_invitations")
    {
        receivedInvitations();
    }
    else if(command == "reports")
    {
        reportsOutput();
    }
    else if(command == "ranked_match_opponents")
    {
        rankedMatchOpponents(setArgs);
    }
    else if(command == "casual_match_opponents")
    {
        casualMatchOpponents(setArgs);
    }
    else
    {
        cout << NOT_FOUND << endl;
    }
}


map<string, string> FastestHand::parseArguments() {
    map<string, string> setArgs;
    string rest_of_line;
    getline(cin, rest_of_line);
    istringstream iss(rest_of_line);
    string key, val;
    while(iss >> key >> quoted(val)) {
        setArgs[key] = val;
    }
    return setArgs;
}


vector<Player>::iterator FastestHand::findPlayerByUsername(string username)
{
    return find_if(players.begin(), players.end(), [&](Player p){
        return p.getUsername() == username;
    });
}


vector<Admin>::iterator FastestHand::findAdminByUsername(string username)
{
    return find_if(admins.begin(), admins.end(), [&](Admin a){
        return a.getUsername() == username;
    });
}


void FastestHand::playerRegister(map<string, string> setArgs)
{
    string username;
    string password;

    map<string, string>::iterator u_it = find_if(setArgs.begin(), setArgs.end(), [](pair<string, string> p){return p.first == "username";});

    map<string, string>::iterator p_it = find_if(setArgs.begin(), setArgs.end(), [](pair<string, string> p){return p.first == "password";});

    if(u_it == setArgs.end() || p_it == setArgs.end())
    {
        cout << BAD_REQUEST << endl;
        return;
    }

    username = u_it->second;
    password = p_it->second;

    if(session.isPlayer == true || session.isAdmin == true)
    {
        cout << PERMISSION_DENIED << endl;
        return;
    }

    if(usernameAlreadyExists(username))
    {
        cout << BAD_REQUEST << endl;
        return;
    }

    
    cout << OK << endl;
    Player newPlayer(username, password);
    players.push_back(newPlayer);
    session.isPlayer = true;
    session.username = username;
}


bool FastestHand::usernameAlreadyExists(string username)
{
    vector<Player>::iterator p_it = findPlayerByUsername(username);

    vector<Admin>::iterator a_it = findAdminByUsername(username);
    
    if(p_it != players.end() || a_it != admins.end())
    {
        return true;
    }
    else
    {
        return false;
    }
}


void FastestHand::login(map<string, string> setArgs)
{
    string username;
    string password;

    map<string, string>::iterator u_it = find_if(setArgs.begin(), setArgs.end(), [](pair<string, string> p){return p.first == "username";});

    map<string, string>::iterator p_it = find_if(setArgs.begin(), setArgs.end(), [](pair<string, string> p){return p.first == "password";});

    if(u_it == setArgs.end() || p_it == setArgs.end())
    {
        cout << BAD_REQUEST << endl;
        return;
    }

    username = u_it->second;
    password = p_it->second;

    if(session.isAdmin == true || session.isPlayer == true)
    {
        cout << PERMISSION_DENIED << endl;
        return;
    }

    if(!usernameAlreadyExists(username))
    {
        cout << NOT_FOUND << endl;
        return;
    }

    if(wrongPassword(username, password))
    {
        cout << PERMISSION_DENIED << endl;
        return;
    }

    vector<Player>::iterator player_it = findPlayerByUsername(username);

    vector<Admin>::iterator admin_it = findAdminByUsername(username);

    if(player_it != players.end())
    {
        if(player_it->getStatus() == LOGGED_IN)
        {
            cout << PERMISSION_DENIED << endl;
            return;
        }
        cout << OK << endl;
        player_it->changeStatus();
        session.isPlayer = true;
        session.username = username;

    }
    else
    {
        if(admin_it->getStatus() == LOGGED_IN)
        {
            cout << PERMISSION_DENIED << endl;
            return;
        }
        cout << OK << endl;
        admin_it->changeStatus();
        session.isAdmin = true;
        session.username = username;
    }
}


bool FastestHand::wrongPassword(string username, string password)
{
    vector<Player>::iterator p_it = findPlayerByUsername(username);

    vector<Admin>::iterator a_it = findAdminByUsername(username);

    if(p_it != players.end())
    {
        if(p_it->getPassword() != password)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        if(a_it->getPassword() != password)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}


void FastestHand::logout()
{
    if(session.isPlayer == false && session.isAdmin == false)
    {
        cout << PERMISSION_DENIED << endl;
        return;
    }

    vector<Player>::iterator player_it = findPlayerByUsername(session.username);

    vector<Admin>::iterator admin_it = findAdminByUsername(session.username);

    if(player_it != players.end())
    {
        cout << OK << endl;
        player_it->changeStatus();
        session.isPlayer = false;
        session.username = "";
    }
    else
    {
        cout << OK << endl;
        admin_it->changeStatus();
        session.isAdmin = false;
        session.username = "";
    }
}


void FastestHand::casualMatchReady(map<string, string> setArgs)
{
    map<string, string>::iterator s_it = find_if(setArgs.begin(), setArgs.end(), [](pair<string, string> p){return p.first == "status";});

    if(s_it == setArgs.end() || (s_it->second != "true" && s_it->second != "false"))
    {
        cout << BAD_REQUEST << endl;
        return;
    }

    string arg2 = s_it->second;

    if(session.isPlayer == false)
    {
        cout << PERMISSION_DENIED << endl;
        return;
    }

    vector<Player>::iterator player_it = findPlayerByUsername(session.username);
    
    if(arg2 == "true")
    {
        player_it->changeReadyStatus(true);
    }
    else if(arg2 == "false")
    {
        player_it->changeReadyStatus(false);
    }
    cout << OK << endl;

}


void FastestHand::casualMatchOpponents(map<string, string> setArgs)
{
    string sort_order = "desc"; 

    if(!setArgs.empty())
    {
        map<string, string>::iterator s_it = find_if(setArgs.begin(), setArgs.end(), [](pair<string, string> p){return p.first == "sort_order";});

        if(s_it == setArgs.end() || (s_it->second != "asc" && s_it->second != "desc"))
        {
            cout << BAD_REQUEST << endl;
            return;
        }
        
        sort_order = s_it->second;
    }

    if(session.isPlayer == false)
    {
        cout << PERMISSION_DENIED << endl;
        return;
    }

    vector<Player> opponents = inOrderReadyPlayers(sort_order);

    if(opponents.size() == 0)
    {
        cout << EMPTY << endl;
        return;
    }

    int player_number = 1;

    for(auto player : opponents)
    {
        cout << player_number << ". " << player.getUsername() << " with " << player.getXP() << " XP" << endl;
        player_number++;
    }
}


vector<Player> FastestHand::inOrderReadyPlayers(string order_type)
{
    vector<Player> in_order_players;

    for(auto player : players)
    {
        if(player.getReadyStatus() == true && player.getUsername() != session.username)
        {
            in_order_players.push_back(player);
        }
    }

    if(order_type == "asc")
    {
        sort(in_order_players.begin(), in_order_players.end(), [](Player a, Player b){
            if(a.getXP() == b.getXP())
            {
                return a.getUsername() < b.getUsername();
            }
            return a.getXP() < b.getXP();
        });
    }
    else if(order_type == "desc")
    {
        sort(in_order_players.begin(), in_order_players.end(), [](Player a, Player b){
            if(a.getXP() == b.getXP())
            {
                return a.getUsername() < b.getUsername();
            }
            return a.getXP() > b.getXP();
        });
    }
    return in_order_players;
}


void FastestHand::invitation(map<string, string> setArgs)
{
    string username;
    string match_type;

    map<string, string>::iterator u_it = find_if(setArgs.begin(), setArgs.end(), [](pair<string, string> u){return u.first == "username";});

    map<string, string>::iterator m_it = find_if(setArgs.begin(), setArgs.end(), [](pair<string, string> m){return m.first == "match_type";});

    if(u_it == setArgs.end() || m_it == setArgs.end())
    {
        cout << BAD_REQUEST << endl;
        return;
    }

    if(m_it->second != CASUAL && m_it->second != RANKED)
    {
        cout << BAD_REQUEST << endl;
        return;
    }

    if(session.isPlayer == false)
    {
        cout << PERMISSION_DENIED << endl;
        return;
    }

    username = u_it->second;
    match_type = m_it->second;

    vector<Admin>::iterator admin_it = findAdminByUsername(username);

    if(admin_it != admins.end())
    {
        cout << PERMISSION_DENIED << endl;
        return;
    }

    vector<Player>::iterator player_it  = findPlayerByUsername(username);

    if(player_it == players.end())
    {
        cout << NOT_FOUND << endl;
        return;
    }

    if(invitedPlayerBlockedYou(username))
    {
        cout << NOT_FOUND << endl;
        return;
    }

    cout << OK << endl;

    inviteCreator(match_type, username);

}


bool FastestHand::invitedPlayerBlockedYou(string invited)
{
    vector<Player>::iterator invited_player = findPlayerByUsername(invited);

    vector<string>::iterator blocked_player = find_if(invited_player->getBlockedPlayers().begin(), invited_player->getBlockedPlayers().end(), [&](string s){return s == session.username;});

    if(blocked_player != invited_player->getBlockedPlayers().end())
    {
        return true;
    }
    return false;
}


void FastestHand::inviteCreator(string match_type, string invited)
{
    Invitation newInvitation;
    newInvitation.inviter = session.username;
    newInvitation.invited = invited;
    newInvitation.match_type = match_type;
    newInvitation.id = invitation_id_;
    invitations.push_back(newInvitation);
    invitation_id_++;
}


void FastestHand::startMatch(map<string, string> setArgs)
{
    map<string, string>::iterator id_it = find_if(setArgs.begin(), setArgs.end(), [](pair<string, string> p){return p.first == "invitation_id";});

    if(id_it == setArgs.end())
    {
        cout << BAD_REQUEST << endl;
        return;
    }

    int invitation_id = stoi(id_it->second);

    vector<Invitation>::iterator i_it = find_if(invitations.begin(), invitations.end(), [&](Invitation i){return i.id == invitation_id;});


    if(session.isPlayer == false)
    {
        cout << PERMISSION_DENIED << endl;
        return;
    }

    if(i_it == invitations.end())
    {
        cout << NOT_FOUND << endl;
        return;
    }

    if(session.username != i_it->invited)
    {
        cout << PERMISSION_DENIED << endl;
        return;
    }


    vector<Player>::iterator p1_it = findPlayerByUsername(i_it->inviter);

    vector<Player>::iterator p2_it = findPlayerByUsername(i_it->invited);

    if(p1_it->getPlayingStatus() == true || p2_it->getPlayingStatus() == true)
    {
        cout << PERMISSION_DENIED << endl;
        return;
    }

    invitations.erase(remove_if(invitations.begin(), invitations.end(), [&](Invitation i){
        return i.id == invitation_id;
    }));


    if(i_it->match_type == CASUAL)
    {
        startCasualMatch(&(*p1_it), &(*p2_it), i_it->id);
    }
    else if(i_it->match_type == RANKED)
    {
        if(p1_it->getRankedLevel() != p2_it->getRankedLevel())
        {
            cout << LEVEL_MISMATCH << endl;
            return;
        }
        startRankedMatch(&(*p1_it), &(*p2_it), i_it->id);
    }
    cout << OK << endl;
}


void FastestHand::startCasualMatch(Player *inviter, Player *invited, int invitation_id)
{
    Casual newCasualMatch(inviter->getUsername(), invited->getUsername(), invitation_id);
    casual_matches.push_back(newCasualMatch);
    inviter->changeMatchType(CASUAL);
    inviter->changePlayingStatus(true);
    invited->changeMatchType(CASUAL);
    invited->changePlayingStatus(true);
    inviter->startCasualGame();
    invited->startCasualGame();
}


void FastestHand::startRankedMatch(Player *inviter, Player *invited, int invitation_id)
{
    Ranked newRankedMatch(inviter->getUsername(), invited->getUsername(), invitation_id);
    ranked_matches.push_back(newRankedMatch);
    inviter->changeMatchType(RANKED);
    inviter->changePlayingStatus(true);
    invited->changeMatchType(RANKED);
    invited->changePlayingStatus(true);
    inviter->startRankedGame();
    invited->startRankedGame();
}


void FastestHand::rejectInvitation(map<string, string> setArgs)
{
    map<string, string>::iterator id_it = find_if(setArgs.begin(), setArgs.end(), [](pair<string, string> p){return p.first == "invitation_id";});

    if(id_it == setArgs.end())
    {
        cout << BAD_REQUEST << endl;
        return;
    }

    int invitation_id = stoi(id_it->second);

    vector<Invitation>::iterator i_it = find_if(invitations.begin(), invitations.end(), [&](Invitation i){return i.id == invitation_id;});

    if(session.isPlayer == false)
    {
        cout << PERMISSION_DENIED << endl;
        return;
    }


    if(i_it == invitations.end())
    {
        cout << NOT_FOUND << endl;
        return;
    }

    if(session.username != i_it->invited)
    {
        cout << PERMISSION_DENIED << endl;
        return;
    }


    cout << OK << endl;
    
    invitations.erase(remove_if(invitations.begin(), invitations.end(), [&](Invitation i){
        return i.id == invitation_id;
    }));
}


void FastestHand::action(map<string, string> setArgs)
{
    map<string, string>::iterator a_it = find_if(setArgs.begin(), setArgs.end(), [](pair<string, string> p){return p.first == "action";});

    if(a_it == setArgs.end() || (a_it->second != SHOOT && a_it->second != DEFEND && a_it->second != RELOAD))
    {
        cout << BAD_REQUEST << endl;
        return;
    }

    string act = a_it->second;

    if(session.isPlayer == false)
    {
        cout << PERMISSION_DENIED << endl;
        return;
    }


    vector<Casual>::iterator casual_it = find_if(casual_matches.begin(), casual_matches.end(), [&](Casual c){return (c.getInviter() == session.username || c.getInvited() == session.username) && c.isFinished() == false;});

    vector<Ranked>::iterator ranked_it = find_if(ranked_matches.begin(), ranked_matches.end(), [&](Ranked r){return (r.getInviter() == session.username || r.getInvited() == session.username) && r.isFinished() == false;});

    if(casual_it == casual_matches.end() && ranked_it == ranked_matches.end())
    {
        cout << NOT_FOUND << endl;
        return;
    }

    if(casual_it != casual_matches.end())
    {
        Player* current_player = &(*findPlayerByUsername(session.username));
        Player* other_player;

        if(casual_it->getInvited() == current_player->getUsername())
        {
            other_player = &(*findPlayerByUsername(casual_it->getInviter()));
        }
        else
        {
            other_player = &(*findPlayerByUsername(casual_it->getInvited()));
        }
        casual_it->performAction(current_player, other_player, act);
    }
    else if(ranked_it != ranked_matches.end())
    {
        Player* current_player = &(*findPlayerByUsername(session.username));
        Player* other_player;

        if(ranked_it->getInvited() == current_player->getUsername())
        {
            other_player = &(*findPlayerByUsername(ranked_it->getInviter()));
        }
        else
        {
            other_player = &(*findPlayerByUsername(ranked_it->getInvited()));
        }
        ranked_it->performAction(current_player, other_player, act);
    }

}


void FastestHand::matchStatus()
{
    if(session.isPlayer == false)
    {
        cout << PERMISSION_DENIED << endl;
        return;
    }

    vector<Casual>::iterator casual_it = find_if(casual_matches.begin(), casual_matches.end(), [&](Casual c){return (c.getInvited() == session.username || c.getInviter() == session.username) && c.isFinished() == false;});

    vector<Ranked>::iterator ranked_it = find_if(ranked_matches.begin(), ranked_matches.end(), [&](Ranked r){return (r.getInvited() == session.username || r.getInviter() == session.username) && r.isFinished() == false;});

    if(casual_it == casual_matches.end() && ranked_it == ranked_matches.end())
    {
        cout << NOT_FOUND << endl;
        return;
    }

    vector<Player>::iterator current_player = findPlayerByUsername(session.username);

    vector<Player>::iterator other_player = findOtherPlayerForMatchStatus(ranked_it, casual_it, current_player);

    if(casual_it != casual_matches.end())
    {
        casualMatchStatusOutput((*current_player), (*other_player), (*casual_it));
    }
    else if(ranked_it != ranked_matches.end())
    {
        rankedMatchStatusOutput((*current_player), (*other_player), (*ranked_it));
    }
}


vector<Player>::iterator FastestHand::findOtherPlayerForMatchStatus(vector<Ranked>::iterator ranked_it, vector<Casual>::iterator casual_it, vector<Player>::iterator current_player)
{
    vector<Player>::iterator other_player;

    if(casual_it != casual_matches.end())
    {
        if(casual_it->getInvited() == current_player->getUsername())
        {
            other_player = findPlayerByUsername(casual_it->getInviter());
        }
        else if(casual_it->getInviter() == current_player->getUsername())
        {
            other_player = findPlayerByUsername(casual_it->getInvited());
        }   
    }
    else if(ranked_it != ranked_matches.end())
    {
        if(ranked_it->getInvited() == current_player->getUsername())
        {
            other_player = findPlayerByUsername(ranked_it->getInviter());
        }
        else if(ranked_it->getInviter() == current_player->getUsername())
        {
            other_player = findPlayerByUsername(ranked_it->getInvited());
        }   
    }
    return other_player;
}



void FastestHand::casualMatchStatusOutput(Player current_player, Player other_player, Casual match)
{
    cout << "Turn " << match.getTurnNumber() << endl;
    if(current_player.getCurrentCasualAct() == "")
    {
        cout << "You: pending" << endl;
    }
    else
    {
        cout << "You: " << current_player.getCurrentCasualAct() << endl;
    }

    if(other_player.getCurrentCasualAct() == "")
    {
        cout << "Your opponent: pending" << endl;
    }
    else
    {
        cout << "Your opponent: played" << endl;
    }

    cout << "History:" << endl;
    cout << left << setw(20) << "Opponent's moves:" << "Your moves:" << endl;
    if(match.getTurnNumber() != 1)
    {
        for(int i = 0, j = 0; i < current_player.getCasualActions().size() && j < other_player.getCasualActions().size(); i++ && j++)
        {
            cout << left << setw(20) << other_player.getCasualActions()[j] << current_player.getCasualActions()[i] << endl;
        }
    }
    cout << "Your remaining bullets: " << current_player.getRemainingCasualBullets() << endl;
}


void FastestHand::rankedMatchStatusOutput(Player current_player, Player other_player, Ranked match)
{
    cout << "Turn " << match.getTurnNumber() << endl;
    if(current_player.getCurrentRankedAct() == "")
    {
        cout << "You: pending" << endl;
    }
    else
    {
        cout << "You: " << current_player.getCurrentRankedAct() << endl;
    }

    if(other_player.getCurrentRankedAct() == "")
    {
        cout << "Your opponent: pending" << endl;
    }
    else
    {
        cout << "Your opponent: played" << endl;
    }

    cout << "History:" << endl;
    cout << left << setw(20) << "Opponent's moves:" << "Your moves:" << endl;
    if(match.getTurnNumber() != 1)
    {
        for(int i = 0, j = 0; i < current_player.getRankedActions().size() && j < other_player.getRankedActions().size(); i++, j++)
        {
            cout << left << setw(20) << other_player.getRankedActions()[j] << current_player.getRankedActions()[i] << endl;
        }
    }
    cout << "Your remaining bullets: " << current_player.getRankedGameStatus().bullets << endl;
    cout << "Your remaining health: " << current_player.getCurrentRankedHealth() << endl;
}


void FastestHand::report(map<string, string> setArgs)
{
    string username;
    string reason;

    map<string, string>::iterator u_it = find_if(setArgs.begin(), setArgs.end(), [](pair<string, string> p){return p.first == "username";});

    map<string, string>::iterator r_it = find_if(setArgs.begin(), setArgs.end(), [](pair<string, string> p){return p.first == "reason";});


    if(u_it == setArgs.end() || r_it == setArgs.end())
    {
        cout << BAD_REQUEST << endl;
        return;
    }

    if(session.isPlayer == false)
    {
        cout << PERMISSION_DENIED << endl;
        return;
    }

    username = u_it->second;
    reason = r_it->second;

    vector<Player>::iterator p_it = findPlayerByUsername(username);

    if(p_it == players.end())
    {
        cout << NOT_FOUND << endl;
        return;
    }

    if(reason.size() == 0)
    {
        cout << BAD_REQUEST << endl;
        return;
    }


    cout << OK << endl;
    Report new_report;
    new_report.reporter = session.username;
    new_report.reported = username;
    new_report.reason = reason;
    new_report.id = report_id_;
    report_id_++;
    reports.push_back(new_report);
}


void FastestHand::profile(map<string, string> setArgs)
{
    if(setArgs.empty())
    {
        if(session.isPlayer == false)
        {
            cout << PERMISSION_DENIED << endl;
            return;
        }
        myProfile();
    }
    else
    {
        map<string, string>::iterator u_it = find_if(setArgs.begin(), setArgs.end(), [](pair<string, string> p){return p.first == "username";});

        if(u_it == setArgs.end())
        {
            cout << BAD_REQUEST << endl;
            return;
        }

        string username = u_it->second;

        if(session.isPlayer == false && session.isAdmin == false)
        {
            cout << PERMISSION_DENIED << endl;
            return;
        }

        vector<Admin>::iterator admin_it = findAdminByUsername(username);

        if(admin_it != admins.end())
        {
            cout << PERMISSION_DENIED << endl;
            return;
        }

        othersProfile(username);
    }
    
}


void FastestHand::myProfile()
{
    vector<Player>::iterator user_it = findPlayerByUsername(session.username);

    cout << "username: \"" << user_it->getUsername() << "\"" << endl;
    cout << "Level: " << user_it->getRankedLevel() << endl;
    cout << "RP: " << user_it->getRP() << endl;
    cout << "XP: " << user_it->getXP() << endl;
    cout << "Total wins: " << totalWins(user_it->getUsername()) << endl;
    cout << "Total losses: " << totalLosses(user_it->getUsername()) << endl; 
}


void FastestHand::othersProfile(string username)
{
    vector<Player>::iterator user_it = findPlayerByUsername(username);

    if(user_it == players.end())
    {
        cout << NOT_FOUND << endl;
        return;
    }

    cout << "username: \"" << user_it->getUsername() << "\"" << endl;
    cout << "Level: " << user_it->getRankedLevel() << endl;
    cout << "RP: " << user_it->getRP() << endl;
    cout << "XP: " << user_it->getXP() << endl;
    cout << "Total wins: " << totalWins(user_it->getUsername()) << endl;
    cout << "Total losses: " << totalLosses(user_it->getUsername()) << endl; 
}


int FastestHand::totalWins(string username)
{
    int total_wins = 0;

    for(Casual match : casual_matches)
    {
        if(match.getWinner() == username)
        {
            total_wins++;
        }
    }

    for(Ranked match : ranked_matches)
    {
        if(match.getWinner() == username)
        {
            total_wins++;
        }
    }
    return total_wins;
}


int FastestHand::totalLosses(string username)
{
    int total_losses = 0;

    for(Casual match : casual_matches)
    {
        if(match.getLoser() == username)
        {
            total_losses++;
        }
    }

    for(Ranked match : ranked_matches)
    {
        if(match.getLoser() == username)
        {
            total_losses++;
        }
    }
    return total_losses;
}


void FastestHand::receivedInvitations()
{
    if(session.isPlayer == false)
    {
        cout << PERMISSION_DENIED << endl;
        return;
    }

    if(inOrderInvitations(session.username).size() == 0)
    {
        cout << EMPTY << endl;
        return;
    }
    else
    {
        for(Invitation invite : inOrderInvitations(session.username))
        {
            if(invite.match_type == CASUAL)
            {
                cout << invite.id << ": Invitation from \"" << invite.inviter << "\" for a \"casual\" match" << endl;
            }
            else if(invite.match_type == RANKED)
            {
                cout << invite.id << ": Invitation from \"" << invite.inviter << "\" for a \"ranked\" match" << endl;
            }
        }
    }
}


vector<Invitation> FastestHand::inOrderInvitations(string username)
{
    vector<Invitation> in_order_invitations;

    for(Invitation invite : invitations)
    {
        if(invite.invited == username)
        {
            in_order_invitations.push_back(invite);
        }
    }

    sort(in_order_invitations.begin(), in_order_invitations.end(), [](Invitation a, Invitation b){
        return a.id < b.id;
    });

    return in_order_invitations;
}


void FastestHand::reportsOutput()
{
    if(session.isAdmin == false)
    {
        cout << PERMISSION_DENIED << endl;
        return;
    }

    if(inOrderReports().size() == 0)
    {
        cout << EMPTY << endl;
        return;
    }
    else
    {
        for(Report report : inOrderReports())
        {
            cout << report.id << ": \"" << report.reporter << "\" reported \"" << report.reported << "\" for: \"" << report.reason << "\"" << endl;
        }
    }
}


vector<Report> FastestHand::inOrderReports()
{
    vector<Report> in_order_reports = reports;
    sort(in_order_reports.begin(), in_order_reports.end(), [](Report a, Report b){
        return a.id < b.id;
    });
    return in_order_reports;
}


void FastestHand::rankedMatchOpponents(map<string, string> setArgs)
{
    if(!setArgs.empty())
    {
        map<string, string>::iterator s_it = find_if(setArgs.begin(), setArgs.end(), [](pair<string, string> p){return p.first == "sort_order";});

        if(s_it == setArgs.end() || (s_it->second != "desc" && s_it->second != "asc"))
        {
            cout << BAD_REQUEST << endl;
            return;
        }

        string order_type = s_it->second;

        if(session.isPlayer == false)
        {
            cout << PERMISSION_DENIED << endl;
            return;
        }

        if(inOrderRankedPlayers(order_type).size() == 0)
        {
            cout << EMPTY << endl;
            return;
        }

        outputRankedPlayers(inOrderRankedPlayers(order_type));
        
    }
    else
    {
        if(session.isPlayer == false)
        {
            cout << PERMISSION_DENIED << endl;
            return;
        }

        if(inOrderRankedPlayers("desc").size() == 0)
        {
            cout << EMPTY << endl;
            return;
        }

        outputRankedPlayers(inOrderRankedPlayers("desc"));
    }
}


vector<Player> FastestHand::inOrderRankedPlayers(string order_type)
{
    vector<Player>::iterator current_user = findPlayerByUsername(session.username);

    vector<Player> in_order_players;
    
    for(auto player : players)
    {
        if(player.getRankedLevel() == current_user->getRankedLevel())
        {
            if(player.getUsername() == session.username)
            {
                continue;
            }
            in_order_players.push_back(player);
        }
    }
    if(order_type == "desc")
    {
        sort(in_order_players.begin(), in_order_players.end(), [](Player a, Player b){
            if(a.getRP() == b.getRP())
            {
                return a.getUsername() < b.getUsername();
            }
            return a.getRP() > b.getRP();
        });
    }
    else if(order_type == "asc")
    {
        sort(in_order_players.begin(), in_order_players.end(), [](Player a, Player b){
            if(a.getRP() == b.getRP())
            {
                return a.getUsername() < b.getUsername();
            }
            return a.getRP() < b.getRP();
        });
    }

    return in_order_players;
}


void FastestHand::outputRankedPlayers(vector<Player> ranked_players)
{
    int player_number = 1;
    for(auto ranked_player : ranked_players)
    {
        if(ranked_player.getUsername() == session.username)
        {
            continue;
        }
        cout << player_number << ". " << ranked_player.getUsername() << " with " << ranked_player.getRP() << " RP" << endl;
        player_number++;
    }
}


void FastestHand::block(map<string, string> setArgs)
{
    map<string, string>::iterator u_it = find_if(setArgs.begin(), setArgs.end(), [](pair<string, string> p){return p.first == "username";});

    map<string, string>::iterator s_it = find_if(setArgs.begin(), setArgs.end(), [](pair<string, string> p){return p.first == "status";});

    if(u_it == setArgs.end() || s_it == setArgs.end())
    {
        cout << BAD_REQUEST << endl;
        return;
    }

    string username = u_it->second;
    string status = s_it->second;

    if(session.isPlayer == false)
    {
        cout << PERMISSION_DENIED << endl;
        return;
    }

    if(status != BLOCKED && status != UNBLOCKED)
    {
        cout << BAD_REQUEST << endl;
        return;
    }

    if(usernameNotFound(username))
    {
        cout << NOT_FOUND << endl;
        return;
    }

    if(adminUsername(username))
    {
        cout << BAD_REQUEST << endl;
        return;
    }

    cout << OK << endl;

    vector<Player>::iterator current_user = findPlayerByUsername(session.username);

    current_user->blockPlayer(username);
}


bool FastestHand::usernameNotFound(string username)
{
    vector<Player>::iterator player_it = findPlayerByUsername(username);

    if(player_it == players.end())
    {
        return true;
    }
    return false;
}

bool FastestHand::adminUsername(string username)
{
    vector<Admin>::iterator admin_it = findAdminByUsername(username);

    if(admin_it != admins.end())
    {
        return true;
    }
    return false;
}


void FastestHand::penalty(map<string, string> setArgs)
{
    if(session.isAdmin == false)
    {
        cout << PERMISSION_DENIED << endl;
        return;
    }

    int report_id, amount, number_of_matches;
    string type;
    map<string, string>::iterator id_it = find_if(setArgs.begin(), setArgs.end(), [](pair<string, string> p){return p.first == "report_id";});
    map<string, string>::iterator type_it = find_if(setArgs.begin(), setArgs.end(), [](pair<string, string> p){return p.first == "type";});
    map<string, string>::iterator amount_it = find_if(setArgs.begin(), setArgs.end(), [](pair<string, string> p){return p.first == "amount";});
    map<string, string>::iterator num_it = find_if(setArgs.begin(), setArgs.end(), [](pair<string, string> p){return p.first == "number_of_matches";});

    if(id_it == setArgs.end() || type_it == setArgs.end() || amount_it == setArgs.end() || num_it == setArgs.end())
    {
        cout << BAD_REQUEST << endl;
        return;
    }

    report_id = stoi(id_it->second);
    type = type_it->second;
    amount = stoi(amount_it->second);
    number_of_matches = stoi(num_it->second);
    if((type != "health_penalty" && type != "bullet_penalty") || outOfRangeAmount(type, amount) || number_of_matches < 1)
    {
        cout << BAD_REQUEST << endl;
        return;
    }

    vector<Report>::iterator report_it = find_if(reports.begin(), reports.end(), [&](Report r){return r.id == report_id;});
    if(report_it == reports.end())
    {
        cout << NOT_FOUND << endl;
        return;
    }
    cout << OK << endl;
    vector<Player>::iterator punished_player = findPlayerByUsername(report_it->reported);
    punished_player->penalize(type, amount, number_of_matches);
    reports.erase(remove_if(reports.begin(), reports.end(), [&](Report r){return r.id == report_id;}));

}


bool FastestHand::outOfRangeAmount(string type, int amount)
{
    if(type == "health_penalty")
    {
        if(amount < 1 || amount > 2)
        {
            return true;
        }
        return false;
    }
    else if(type == "bullet_penalty")
    {
        if(amount < 1 || amount > 3)
        {
            return true;
        }
        return false;
    }
    return false;
}


void FastestHand::dismissReport(map<string, string> setArgs)
{
    map<string, string>::iterator id_it = find_if(setArgs.begin(), setArgs.end(), [](pair<string, string> p){return p.first == "report_id";});

    if(id_it == setArgs.end() || id_it->second.empty())
    {
        cout << BAD_REQUEST << endl;
        return;
    }

    int report_id = stoi(id_it->second);

    if(session.isAdmin == false)
    {
        cout << PERMISSION_DENIED << endl;
        return;
    }

    vector<Report>::iterator report_it = find_if(reports.begin(), reports.end(), [&](Report r){return r.id == report_id;});

    if(report_it == reports.end())
    {
        cout << NOT_FOUND << endl;
        return;
    }
    cout << OK << endl;
    reports.erase(remove_if(reports.begin(), reports.end(), [&](Report r){return r.id == report_id;}));
}