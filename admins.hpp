#ifndef ADMINS_HPP
#define ADMINS_HPP
#include <iostream>
using namespace std;


class Admin
{
public:
    Admin(string username, string password);
    string getUsername();
    string getPassword();
    string getStatus();
    void changeStatus();
private:
    string username_;
    string password_;
    string status_;
};

#endif