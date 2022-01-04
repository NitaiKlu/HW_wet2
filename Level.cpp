#include "Level.h"

bool Level::isEmpty() const
{
    return players.getCount() == 0;
}

bool Level::isPlayerExist(Id id) const
{
    return players.isExist(id);
}

Player_ptr Level::getPlayerPtr(Id id)
{
    if(!players.isExist(id)) 
    {
        return nullptr;
    }
    return players.find(id);
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