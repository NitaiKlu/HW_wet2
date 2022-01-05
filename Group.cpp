#include "Group.h"

bool Group::isLevelExist(int level) const
{
    return levels.isExist(level);
}

bool Group::isEmpty() const
{
    return num_of_players == 0;
}

Level_ptr Group::getLevelPtr(int level)
{
    return levels.getData(level);
}

int Group::getNumOfPlayers() const
{
    return num_of_players;
}

Status Group::addPlayer(Id id, Player_ptr player)
{
    int level = player->getLevel();
    if (!levels.isExist(level)) //this level doesn't exist
    {
        levels.insert(level, make_shared<Level>());
    }
    // level defintely exists by now
    Level_ptr players_of_this_level = levels.getData(level);
    players_of_this_level->addPlayer(player);
    num_of_players++;
    return S_SUCCESS;
}

void Group::addLevel(Level_ptr new_level)
{
    levels.insert(new_level->getLevel(), new_level);
}

/**
 * 
 * assumes the player exists in the group.
 * */
Status Group::removePlayer(Id id, Player_ptr player)
{
    int level = player->getLevel();
    Level_ptr players_of_this_level = levels.getData(level);
    players_of_this_level->removePlayer(id);
    num_of_players--;
    // checking if the level is now empty
    if (players_of_this_level->isEmpty())
    {
        levels.remove(level);
    }
    return S_SUCCESS;
}

Status Group::getPercentOfPlayersWithScoreInBounds(int score, int lowerLevel, int higherLevel, double * players)
{
    /**********************************************
     * needed searchFromBelow
     * needed searchFromAbove
     * number of elements in the i-shRank between 2 levels in the tree
     * seperate function to get all players between 2 levels in the tree
     * ********************************************/
    int low_level;
    int high_level;
    int score_low_rank = levels.Rank(low_level, score);
    int score_high_rank = levels.Rank(high_level, score);

    return S_SUCCESS;
}