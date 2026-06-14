#include <iostream>
#include "fastestHand.hpp"
using namespace std;

int main(int argc, char* argv[])
{
    string playersCSV;
    string adminsCSV;

    playersCSV = argv[1];
    adminsCSV = argv[2];

    FastestHand(playersCSV, adminsCSV);
}