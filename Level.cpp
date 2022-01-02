#include "Level.h"

bool Level::isEmpty() const
{
    return players.getCount() == 0;
}

bool Level::isPlayerExist(Id id) const
{
    return players.isExist(id);
}

Status Level::addPlayer(Player_ptr player)
{
    players.insert(player->getId(), player);
    return S_SUCCESS;
}

Status Level::removePlayer(Id id)
{
    players.remove(id);
    return S_SUCCESS;
}

int Level::getSizeOfLevel() const
{
    return players.getCount();
}

int Level::getLevel() const
{
    return level;
}