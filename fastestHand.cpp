#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <map>
#include <iomanip>
#include "fastestHand.hpp"
#include "players.hpp"
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
#define SHOOT "shoot"
#define DEFEND "defend"
#define RELOAD "reload"
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
    if(command == "register")
    {
        playerRegister();
    }
    else if(command == "login")
    {
        login();
    }
    else if(command == "logout")
    {
        logout();
    }
    else if(command == "casual_match_ready")
    {
        casualMatchReady();
    }
    else if(command == "casual_match_opponents")
    {
        casualMatchOpponents();
    }
    else if(command == "invitation")
    {
        invitation();
    } 
    else if(command == "start_match")
    {
        startMatch();
    }
    else if(command == "reject_invitation")
    {
        rejectInvitation();
    }
    else if(command == "action")
    {
        action();
    }
    else if(command == "report")
    {
        report();
    }
    else
    {
        cout << NOT_FOUND << endl;
    }
}


void FastestHand::get(string command)
{
    if(command == "match_status")
    {
        matchStatus();
    }
    else if(command == "profile")
    {
        profile();
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
        rankedMatchOpponents();
    }
    else
    {
        cout << NOT_FOUND << endl;
    }
}


void FastestHand::playerRegister()
{
    map<string, string> setArgs;
    
    string rest_of_line;
    getline(cin, rest_of_line);
    istringstream iss(rest_of_line);
    string key, val;
    while(iss >> key >> quoted(val))
    {
        setArgs[key] = val;
    }

    string username;
    string password;

    map<string, string>::iterator u_it = find_if(setArgs.begin(), setArgs.end(), [](pair<string, string> p){
        return p.first == "username";
    });

    map<string, string>::iterator p_it = find_if(setArgs.begin(), setArgs.end(), [](pair<string, string> p){
        return p.first == "password";
    });

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
}


bool FastestHand::usernameAlreadyExists(string username)
{
    vector<Player>::iterator p_it = find_if(players.begin(), players.end(), [&](Player p){
        return p.getUsername() == username;
    });

    vector<Admin>::iterator a_it = find_if(admins.begin(), admins.end(), [&](Admin a){
        return a.getUsername() == username;
    });
    
    if(p_it != players.end() || a_it != admins.end())
    {
        return true;
    }
    else
    {
        return false;
    }
}


void FastestHand::login()
{
    map<string, string> setArgs;

    string rest_of_line;
    getline(cin, rest_of_line);
    istringstream iss(rest_of_line);
    string key, val;
    while(iss >> key >> quoted(val))
    {
        setArgs[key] = val;
    }

    string username;
    string password;

    map<string, string>::iterator u_it = find_if(setArgs.begin(), setArgs.end(), [](pair<string, string> p){
        return p.first == "username";
    });

    map<string, string>::iterator p_it = find_if(setArgs.begin(), setArgs.end(), [](pair<string, string> p){
        return p.first == "password";
    });

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

    vector<Player>::iterator player_it = find_if(players.begin(), players.end(), [&](Player p){
        return p.getUsername() == username;
    });

    vector<Admin>::iterator admin_it = find_if(admins.begin(), admins.end(), [&](Admin a){
        return a.getUsername() == username;
    });

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
    vector<Player>::iterator p_it = find_if(players.begin(), players.end(), [&](Player p){
        return p.getUsername() == username;
    });

    vector<Admin>::iterator a_it = find_if(admins.begin(), admins.end(), [&](Admin a){
        return a.getUsername() == username;
    });

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

    vector<Player>::iterator player_it = find_if(players.begin(), players.end(), [&](Player p){
        return p.getUsername() == session.username;
    });

    vector<Admin>::iterator admin_it = find_if(admins.begin(), admins.end(), [&](Admin a){
        return a.getUsername() == session.username;
    });

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


void FastestHand::casualMatchReady()
{
    string arg1;
    string arg2;

    string rest_of_line;
    getline(cin, rest_of_line);
    istringstream iss(rest_of_line);

    iss >> arg1 >> quoted(arg2);

    if(arg1 != "status" || (arg2 != "true" && arg2 != "false"))
    {
        cout << BAD_REQUEST << endl;
        return;
    }

    if(session.isPlayer == false)
    {
        cout << PERMISSION_DENIED << endl;
        return;
    }

    vector<Player>::iterator player_it = find_if(players.begin(), players.end(), [&](Player p){
        return p.getUsername() == session.username;
    });
    
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


void FastestHand::casualMatchOpponents()
{
    string rest_of_line;
    getline(cin, rest_of_line);

    string sort_order = "desc"; 

    bool isEmpty = true;
    for(char c : rest_of_line)
    {
        if(c != ' ' && c != '\t' && c != '\r')
        {
            isEmpty = false;
            break;
        }
    }

    
    if(!isEmpty)
    {
        istringstream iss(rest_of_line);
        string arg1;
        string arg2;
        iss >> arg1 >> quoted(arg2);

        if(arg1 != "sort_order" || (arg2 != "asc" && arg2 != "desc"))
        {
            cout << BAD_REQUEST << endl;
            return;
        }
        
        sort_order = arg2;
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
        if(player.getReadyStatus() == true)
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


void FastestHand::invitation()
{
    map<string, string> setArgs;

    string rest_of_line;
    getline(cin, rest_of_line);
    istringstream iss(rest_of_line);
    string key, val;
    while(iss >> key >> quoted(val))
    {
        setArgs[key] = val;
    }

    string username;
    string match_type;

    map<string, string>::iterator u_it = find_if(setArgs.begin(), setArgs.end(), [](pair<string, string> u){
        return u.first == "username";
    });

    map<string, string>::iterator m_it = find_if(setArgs.begin(), setArgs.end(), [](pair<string, string> m){
        return m.first == "match_type";
    });

    if(u_it == setArgs.end() || m_it == setArgs.end())
    {
        cout << BAD_REQUEST << endl;
        return;
    }

    if(m_it->second != "casual")
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

    vector<Admin>::iterator admin_it = find_if(admins.begin(), admins.end(), [&](Admin a){
        return a.getUsername() == username;
    });

    if(admin_it != admins.end())
    {
        cout << PERMISSION_DENIED << endl;
        return;
    }

    vector<Player>::iterator player_it  = find_if(players.begin(), players.end(), [&](Player p){
        return p.getUsername() == username;
    });

    if(player_it == players.end())
    {
        cout << NOT_FOUND << endl;
        return;
    }

    cout << OK << endl;
    Invitation newInvitation;
    newInvitation.inviter = session.username;
    newInvitation.invited = username;
    newInvitation.match_type = match_type;
    newInvitation.id = invitation_id_;
    invitations.push_back(newInvitation);
    invitation_id_++;
}


void FastestHand::startMatch()
{
    string arg1;
    string str_invitation_id;
    int invitation_id;

    string rest_of_line;
    getline(cin, rest_of_line);
    istringstream iss(rest_of_line);

    iss >> arg1 >> quoted(str_invitation_id);
    if(str_invitation_id != "")
    {
        invitation_id = stoi(str_invitation_id);
    }

    if(arg1 != "invitation_id" || iss.fail())
    {
        cout << BAD_REQUEST << endl;
        return;
    }

    vector<Invitation>::iterator i_it = find_if(invitations.begin(), invitations.end(), [&](Invitation i){
        return i.id == invitation_id && i.isFinished == false;
    });

    if(i_it == invitations.end())
    {
        cout << NOT_FOUND << endl;
        return;
    }


    if(session.isPlayer == false || session.username != i_it->invited)
    {
        cout << PERMISSION_DENIED << endl;
        return;
    }


    if(i_it->isAccepted == true || i_it->isRejected == true)
    {
        cout << PERMISSION_DENIED << endl;
        return;
    }

    vector<Player>::iterator p1_it = find_if(players.begin(), players.end(), [&](Player p1){
        return p1.getUsername() == i_it->inviter;
    });

    vector<Player>::iterator p2_it = find_if(players.begin(), players.end(), [&](Player p2){
        return p2.getUsername() == i_it->invited;
    });

    if(p1_it->getPlayingStatus() == true || p2_it->getPlayingStatus() == true)
    {
        cout << PERMISSION_DENIED << endl;
        return;
    }


    cout << OK << endl;
    p1_it->changeMatchType(i_it->match_type);
    p1_it->changePlayingStatus(true);
    p2_it->changeMatchType(i_it->match_type);
    p2_it->changePlayingStatus(true);
    i_it->isAccepted = true;

    p1_it->startCasualGame();
    p2_it->startCasualGame();
}


void FastestHand::rejectInvitation()
{
    string arg1;
    string str_invitation_id;
    int invitation_id;

    string rest_of_line;
    getline(cin, rest_of_line);
    istringstream iss(rest_of_line);

    iss >> arg1 >> quoted(str_invitation_id);
    if(str_invitation_id != "")
    {
        invitation_id = stoi(str_invitation_id);
    }

    if(arg1 != "invitation_id" || iss.fail())
    {
        cout << BAD_REQUEST << endl;
        return;
    }



    vector<Invitation>::iterator i_it = find_if(invitations.begin(), invitations.end(), [&](Invitation i){
        return i.id == invitation_id;
    });

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

    if(i_it->isAccepted == true || i_it->isRejected == true || i_it->isFinished == true)
    {
        cout << NOT_FOUND << endl;
        return;
    }

    cout << OK << endl;
    i_it->isRejected = true;
}


void FastestHand::action()
{
    string arg1;
    string act;

    string rest_of_line;
    getline(cin, rest_of_line);
    istringstream iss(rest_of_line);

    iss >> arg1 >> quoted(act);

    if(arg1 != "action" || (act != SHOOT && act != DEFEND && act != RELOAD))
    {
        cout << BAD_REQUEST << endl;
        return;
    }

    if(session.isPlayer == false)
    {
        cout << PERMISSION_DENIED << endl;
        return;
    }


    vector<Invitation>::iterator i_it = find_if(invitations.begin(), invitations.end(), [&](Invitation i){
        return i.isAccepted == true && (i.invited == session.username || i.inviter == session.username);
    });


    if(i_it == invitations.end())
    {
        cout << NOT_FOUND << endl;
        return;
    }

    performAction(&(*i_it), act);
}


void FastestHand::performAction(Invitation* invite, string act)
{
    vector<Player>::iterator current_player = find_if(players.begin(), players.end(), [&](Player p){
        return p.getUsername() == session.username;
    });


    if(current_player->getCurrentAct().size() != 0)
    {
        cout << PERMISSION_DENIED << endl;
        return;
    }
    if(invite->invited == current_player->getUsername())
    {
        vector<Player>::iterator other_player = find_if(players.begin(), players.end(), [&](Player p){
            return p.getUsername() == invite->inviter;
        });

        if(act == SHOOT)
        {
            if(current_player->getCasualGameStatus().bullets == 0)
            {
                cout << BAD_REQUEST << endl;
                return;
            }
            if(casualShoot(invite, &(*current_player), &(*other_player)))
            {
                invite->isFinished = true;
                invite->winner = current_player->getUsername();
                invite->loser = other_player->getUsername();
                double match_xp = casualXP((*current_player), (*other_player));
                current_player->increaseXP(match_xp);
                other_player->decreaseXP(match_xp);
                current_player->endCasualGame();
                other_player->endCasualGame();
            }
        }
        else if(act == RELOAD)
        {
            if(!casualReload(invite, &(*current_player), &(*other_player)))
            {
                invite->isFinished = true;
                invite->winner = other_player->getUsername();
                invite->loser = current_player->getUsername();
                double match_xp = casualXP((*current_player), (*other_player));
                current_player->decreaseXP(match_xp);
                other_player->increaseXP(match_xp);
                current_player->endCasualGame();
                other_player->endCasualGame();
            }
        }
        else if(act == DEFEND)
        {
            casualDefend(invite, &(*current_player), &(*other_player));
        }
    }
    else if(invite->inviter == current_player->getUsername())
    {
        vector<Player>::iterator other_player = find_if(players.begin(), players.end(), [&](Player p){
            return p.getUsername() == invite->invited;
        });

        if(act == SHOOT)
        {
            if(current_player->getCasualGameStatus().bullets == 0)
            {
                cout << BAD_REQUEST << endl;
                return;
            }
            if(casualShoot(invite, &(*current_player), &(*other_player)))
            {
                invite->isFinished = true;
                invite->winner = current_player->getUsername();
                invite->loser = other_player->getUsername();
                double match_xp = casualXP((*current_player), (*other_player));
                current_player->increaseXP(match_xp);
                other_player->decreaseXP(match_xp);
                current_player->endCasualGame();
                other_player->endCasualGame();
            }
        }
        else if(act == RELOAD)
        {
            if(!casualReload(invite, &(*current_player), &(*other_player)))
            {
                invite->isFinished = true;
                invite->winner = other_player->getUsername();
                invite->loser = current_player->getUsername();
                double match_xp = casualXP((*current_player), (*other_player));
                current_player->decreaseXP(match_xp);
                other_player->increaseXP(match_xp);
                current_player->endCasualGame();
                other_player->endCasualGame();
            }
        }
        else if(act == DEFEND)
        {
            casualDefend(invite, &(*current_player), &(*other_player));
        }
    }
    cout << OK << endl;

}


bool FastestHand::casualShoot(Invitation* invite, Player* current_player, Player* other_player)
{
    current_player->performAction(SHOOT);
    if(other_player->getCasualGameStatus().act == "")
    {
        current_player->changeCasualAct(SHOOT);
        return false;
    }
    else if(other_player->getCasualGameStatus().act == SHOOT)
    {
        current_player->addAct(SHOOT);
        other_player->addAct(SHOOT);
        current_player->changeCasualAct("");
        other_player->changeCasualAct("");
        invite->turn_number++;
        return false;
    }
    else if(other_player->getCasualGameStatus().act == DEFEND)
    {
        current_player->addAct(SHOOT);
        other_player->addAct(DEFEND);
        current_player->changeCasualAct("");
        other_player->changeCasualAct("");
        invite->turn_number++;
        return false;
    }
    else if(other_player->getCasualGameStatus().act == RELOAD)
    {
        current_player->addAct(SHOOT);
        other_player->addAct(RELOAD);
        invite->turn_number++;
        return true;
    }
    return false;
}


bool FastestHand::casualReload(Invitation* invite, Player* current_player, Player* other_player)
{
    current_player->performAction(RELOAD);
    if(other_player->getCasualGameStatus().act == "")
    {
        current_player->changeCasualAct(RELOAD);
        return true;
    }
    else if(other_player->getCasualGameStatus().act == RELOAD)
    {
        current_player->addAct(RELOAD);
        other_player->addAct(RELOAD);
        current_player->changeCasualAct("");
        other_player->changeCasualAct("");
        invite->turn_number++;
        return true;
    }
    else if(other_player->getCasualGameStatus().act == DEFEND)
    {
        current_player->addAct(RELOAD);
        other_player->addAct(DEFEND);
        current_player->changeCasualAct("");
        other_player->changeCasualAct("");
        invite->turn_number++;
        return true;
    }
    else if(other_player->getCasualGameStatus().act == SHOOT)
    {
        current_player->addAct(RELOAD);
        other_player->addAct(SHOOT);
        invite->turn_number++;
        return false;
    }
    return false;
}


void FastestHand::casualDefend(Invitation* invite, Player* current_player, Player* other_player)
{
    if(other_player->getCasualGameStatus().act == "")
    {
        current_player->changeCasualAct(DEFEND);
    }
    else if(other_player->getCasualGameStatus().act == SHOOT)
    {
        current_player->addAct(DEFEND);
        other_player->addAct(SHOOT);
        current_player->changeCasualAct("");
        other_player->changeCasualAct("");
        invite->turn_number++;
    }
    else if(other_player->getCasualGameStatus().act == RELOAD)
    {
        current_player->addAct(DEFEND);
        other_player->addAct(RELOAD);
        current_player->changeCasualAct("");
        other_player->changeCasualAct("");
        invite->turn_number++;
    }
    else if(other_player->getCasualGameStatus().act == DEFEND)
    {
        current_player->addAct(DEFEND);
        other_player->addAct(DEFEND);
        current_player->changeCasualAct("");
        other_player->changeCasualAct("");
        invite->turn_number++;
    }
    
}


double FastestHand::casualXP(Player winner, Player loser)
{
    double xp_difference = 50 - (0.1 * (winner.getXP() - loser.getXP()));

    return (5 > xp_difference) ? 5 : xp_difference;
}


void FastestHand::matchStatus()
{
    if(session.isPlayer == false)
    {
        cout << PERMISSION_DENIED << endl;
        return;
    }

    vector<Invitation>::iterator i_it = find_if(invitations.begin(), invitations.end(), [&](Invitation i){
        return (i.invited == session.username || i.inviter == session.username) && i.isAccepted == true && i.isFinished == false;
    });

    if(i_it == invitations.end())
    {
        cout << NOT_FOUND << endl;
        return;
    }

    vector<Player>::iterator current_player = find_if(players.begin(), players.end(), [&](Player p){
        return p.getUsername() == session.username;
    });

    vector<Player>::iterator other_player;

    if(i_it->invited == current_player->getUsername())
    {
        other_player = find_if(players.begin(), players.end(), [&](Player p){
            return p.getUsername() == i_it->inviter;
        });
    }
    else if(i_it->inviter == current_player->getUsername())
    {
        other_player = find_if(players.begin(), players.end(), [&](Player p){
            return p.getUsername() == i_it->invited;
        });
    }

    matchStatusOutput((*current_player), (*other_player), (*i_it));
}


void FastestHand::matchStatusOutput(Player current_player, Player other_player, Invitation match)
{
    cout << "Turn " << match.turn_number << endl;
    if(current_player.getCurrentAct() == "")
    {
        cout << "You: pending" << endl;
    }
    else
    {
        cout << "You: " << current_player.getCurrentAct() << endl;
    }

    if(other_player.getCurrentAct() == "")
    {
        cout << "Your opponent: pending" << endl;
    }
    else
    {
        cout << "Your opponent: played" << endl;
    }

    cout << "History:" << endl;
    cout << left << setw(20) << "Opponent's moves:" << "Your moves:" << endl;
    if(match.turn_number != 1)
    {
        for(int i = 0, j = 0; i < current_player.getActions().size(), j < other_player.getActions().size(); i++, j++)
        {
            cout << left << setw(20) << other_player.getActions()[j] << current_player.getActions()[i] << endl;
        }
    }
    cout << "Your remaining bullets: " << current_player.getRemainingBullets() << endl;
}


void FastestHand::report()
{
    map<string, string> setArgs;

    string rest_of_line;
    getline(cin, rest_of_line);
    istringstream iss(rest_of_line);
    string key, val;
    while(iss >> key >> quoted(val))
    {
        setArgs[key] = val;
    }

    string username;
    string reason;

    map<string, string>::iterator u_it = find_if(setArgs.begin(), setArgs.end(), [](pair<string, string> p){
        return p.first == "username";
    });

    map<string, string>::iterator r_it = find_if(setArgs.begin(), setArgs.end(), [](pair<string, string> p){
        return p.first == "reason";
    });


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

    vector<Player>::iterator p_it = find_if(players.begin(), players.end(), [&](Player p){
        return p.getUsername() == username;
    });

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


void FastestHand::profile()
{
    string rest_of_line;
    string arg1;
    string username;
    getline(cin, rest_of_line);

    bool isEmpty = true;

    for(char c : rest_of_line)
    {
        if(c != ' ' && c != '\t' && c != '\r')
        {
            isEmpty = false;
            break;
        }
    }

    if(isEmpty)
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
        istringstream iss(rest_of_line);
        iss >> arg1 >> quoted(username);

        if(arg1 != "username")
        {
            cout << BAD_REQUEST << endl;
            return;
        }

        if(session.isPlayer == false && session.isAdmin == false)
        {
            cout << PERMISSION_DENIED << endl;
            return;
        }

        vector<Admin>::iterator admin_it = find_if(admins.begin(), admins.end(), [&](Admin a){
            return a.getUsername() == username;
        });

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
    vector<Player>::iterator user_it = find_if(players.begin(), players.end(), [&](Player p){
        return p.getUsername() == session.username;
    });

    cout << "username: \"" << user_it->getUsername() << "\"" << endl;
    cout << "XP: " << user_it->getXP() << endl;
    cout << "Total wins: " << totalWins(user_it->getUsername()) << endl;
    cout << "Total losses: " << totalLosses(user_it->getUsername()) << endl; 
}


void FastestHand::othersProfile(string username)
{
    vector<Player>::iterator user_it = find_if(players.begin(), players.end(), [&](Player p){
        return p.getUsername() == username;
    });

    if(user_it == players.end())
    {
        cout << NOT_FOUND << endl;
        return;
    }

    cout << "username: \"" << user_it->getUsername() << "\"" << endl;
    cout << "XP: " << user_it->getXP() << endl;
    cout << "Total wins: " << totalWins(user_it->getUsername()) << endl;
    cout << "Total losses: " << totalLosses(user_it->getUsername()) << endl; 
}


int FastestHand::totalWins(string username)
{
    int total_wins = 0;

    for(Invitation match : invitations)
    {
        if(match.winner == username)
        {
            total_wins++;
        }
    }
    return total_wins;
}


int FastestHand::totalLosses(string username)
{
    int total_losses = 0;

    for(Invitation match : invitations)
    {
        if(match.loser == username)
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
            cout << invite.id << ": Invitation from \"" << invite.inviter << "\" for a \"casual\" match" << endl;
        }
    }
}


vector<Invitation> FastestHand::inOrderInvitations(string username)
{
    vector<Invitation> in_order_invitations;

    for(Invitation invite : invitations)
    {
        if(invite.invited == username && invite.isAccepted == false && invite.isRejected == false && invite.match_type == "casual")
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


void FastestHand::rankedMatchOpponents()
{
    string rest_of_line;
    getline(cin, rest_of_line);
    string arg1;
    string order_type;

    bool isEmpty = true;
    for(char c : rest_of_line)
    {
        if(c != ' ' && c != '\t' && c != '\r')
        {
            isEmpty = false;
            break;
        }
    }
    
    if(!isEmpty)
    {
        istringstream iss(rest_of_line);
        iss >> arg1 >> quoted(order_type);

        if(arg1 != "sort" || (order_type != "desc" && order_type != "asc"))
        {
            cout << BAD_REQUEST << endl;
            return;
        }

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
    vector<Player> in_order_players = players;
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