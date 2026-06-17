#include <iostream>
#include "games.hpp"
using namespace std;


string Game::getInviter()
{
    return inviter_;
}


string Game::getInvited()
{
    return invited_;
}


void Game::finishGame()
{
    isFinished_ = true;
}


void Game::matchOutcome(string winner, string loser)
{
    winner_ = winner;
    loser_ = loser;
}


bool Game::isFinished()
{
    return isFinished_;
}


string Game::getWinner()
{
    return winner_;
}


string Game::getLoser()
{
    return loser_;
}