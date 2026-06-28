#ifndef ADMINS_HPP
#define ADMINS_HPP
#include <iostream>
#include "users.hpp"
using namespace std;

class Admin : public User
{
public:
    Admin(string username, string password);
};

#endif