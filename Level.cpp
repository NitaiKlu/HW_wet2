#include "Level.h"

bool Level::isEmpty()
{
    return players.getCount() == 0;
}

bool Level::isPlayerExist(Id id)
{
    return players.isExist(id);
}

Status Level::addPlayer(Player& player)
{
    players.insert(player.getId(), player);
    return S_SUCCESS;
}

Status Level::removePlayer(Id id)
{
    players.remove(id);
    return S_SUCCESS;
}

int Level::getSizeOfLevel()
{
    return players.getCount();
}

int Level::getLevel()
{
    return level;
}