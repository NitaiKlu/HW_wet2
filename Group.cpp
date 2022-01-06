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

//assuming player's data is ok (score is in range, level is not negative, etc.)
Status Group::addPlayer(Id id, Player_ptr player)
{
    int level = player->getLevel();
    if (!levels.isExist(level)) //this level doesn't exist
    {
        levels.insert(level, make_shared<Level>());
    }
    // level defintely exists by now
    Level_ptr players_of_this_level = levels.getData(level);
    //adding player to the level
    players_of_this_level->addPlayer(player);
    /**
     * *************************************************
     * */    levels.addToSize(level, player->getScore(), 1);
    //updating number of players in the group
    num_of_players++;
    return S_SUCCESS;
}

//assumes level is empty!!
void Group::addLevel(Level_ptr new_level)
{
    levels.insert(new_level->getLevel(), new_level);
}

//assumes the player exists in the group.
Status Group::removePlayer(Id id, Player_ptr player)
{
    int level = player->getLevel();
    Level_ptr players_of_this_level = levels.getData(level);
    //remove from the level
    players_of_this_level->removePlayer(id);
    //updating size (weight) in levels (RTree)
    /**
     * *************************************************
     * */    levels.addToSize(level, player->getScore(), -1);
    //lowering number of players in the group
    num_of_players--;
    // checking if the level is now empty
    if (players_of_this_level->isEmpty())
    {
        levels.remove(level);
    }
    return S_SUCCESS;
}

//assigns to "players" parameter the *precentage* of players within [lowerLevel, upperLevel] with score = score given
Status Group::getPercentOfPlayersWithScoreInBounds(int score, int lowerLevel, int higherLevel, double * players)
{
    //searching the closest levels that exist in group that are in the range specified
    int low_level = levels.searchFromAbove(lowerLevel);
    int high_level = levels.searchFromBelow(higherLevel);
    //getting the score-ish rank of both levels
    int score_low_rank = levels.RankAt(low_level, score - 1);
    int score_high_rank = levels.RankAt(high_level, score - 1);
    int num_of_players_mentioned = score_high_rank - score_low_rank;
    if(num_of_players_mentioned == 0 || num_of_players == 0) //no players fits the purpose..
    {
        return S_FAILURE;
    }
    *players = num_of_players_mentioned / num_of_players ; 
    return S_SUCCESS;
}

//assumes m > 0 
Status Group::averageHighestPlayerLevelByGroup(int m, double * avgLevel)
{
    if(m > num_of_players)
    {
        return S_FAILURE;
    }
    //searching for m highest leveled players in the group
    int lower_m_level = levels.sumSelectFromAbove(num_of_players - m, num_of_players - m);
    int num_of_levels_between; //needs to be another weight of sum of levels ... :(
    /**
     * decide what to do if the number isn't exactly m ?
     * */

    //

    return S_SUCCESS;
}