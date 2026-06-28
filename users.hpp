#ifndef USERS_HPP
#define USERS_HPP
#include <iostream>
#define LOGGED_IN "logged in"
#define LOGGED_OUT "logged out"
using namespace std;

class User
{
public:
    User(string username, string password);
    string getUsername();
    string getPassword();
    string getStatus();
    void changeStatus();
protected:
    string username_;
    string password_;
    string status_;
};

#endif