#include <iostream>
#include "casualGame.hpp"
using namespace std;


void Casual::increaseTurnNumber()
{
    turn_number_++;
}


void Casual::decreaseTurnNumber()
{
    turn_number_--;
}


int Casual::getTurnNumber()
{
    return turn_number_;
}