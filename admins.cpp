#include <iostream>
#include "admins.hpp"
#define LOGGED_IN "logged in"
#define LOGGED_OUT "logged out"
using namespace std;


Admin::Admin(string username, string password)
{
    username_ = username;
    password_ = password;
    status_ = LOGGED_OUT;
}


string Admin::getUsername()
{
    return username_;
}


string Admin::getStatus()
{
    return status_;
}


void Admin::changeStatus()
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


string Admin::getPassword()
{
    return password_;
}