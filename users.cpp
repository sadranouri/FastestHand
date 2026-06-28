#include <iostream>
#include "users.hpp"
using namespace std;

User::User(string username, string password)
{
    username_ = username;
    password_ = password;
    status_ = LOGGED_OUT;
}

string User::getUsername()
{
    return username_;
}

string User::getPassword()
{
    return password_;
}

string User::getStatus()
{
    return status_;
}

void User::changeStatus()
{
    if(status_ == LOGGED_IN)
    {
        status_ = LOGGED_OUT;
    }
    else if(status_ == LOGGED_OUT)
    {
        status_ = LOGGED_IN;
    }
}