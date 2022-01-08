#include "Level.h"

Player_ptr Level::getPlayerAt(int index) const
{
    return players.getDataAt(index);
}

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

int Level::getSizeOfTable() const
{
    return players.getSize();
}

int Level::getLevel() const
{
    return level;
}

void Level::addAllPlayers(Level_ptr level1, Level_ptr level2) //add all players in level1 and level2 to myself
{
    int level1_size = level1->getSizeOfTable();
    int level2_size = level2->getSizeOfTable();
    for(int i = 0; i < level1_size; i++)
    {
        if(level1->getPlayerAt(i)->getId() > 0) //this player actually exists and need to be transferred
            addPlayer(level1->getPlayerAt(i));
    }
    for(int i = 0; i < level2_size; i++)
    {
        if(level2->getPlayerAt(i)->getId() > 0) //this player actually exists and need to be transferred
            addPlayer(level2->getPlayerAt(i));
    }
}
