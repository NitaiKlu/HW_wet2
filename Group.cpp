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

int Group::getNumOfLevels() const
{
    return levels.getTreeSize();
}

int Group::getId() const
{
    return id;
}
int Group::getScale() const
{
    return scale;
}

//assuming player's data is ok (score is in range, level is not negative, etc.)
Status Group::addPlayer(Id id, Player_ptr player)
{
    int level = player->getLevel();
    if (!levels.isExist(level)) //this level doesn't exist
    {
        levels.insert(level, make_shared<Level>(level));
    }
    // getting level zero - costs o(1) since it's left_most
    Level_ptr players_of_this_level = levels.getData(level);
    //adding player to the level
    players_of_this_level->addPlayer(player);
    levels.increaseScore(player->getLevel(), player->getScore());
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
    levels.decreaseScore(level, player->getScore());
    //lowering number of players in the group
    num_of_players--;
    // checking if the level is now empty but not level 0
    if (players_of_this_level->isEmpty() && level != 0)
    {
        levels.remove(level);
    }
    return S_SUCCESS;
}

//assigns to "players" parameter the *precentage* of players within [lowerLevel, upperLevel] with score = score given
Status Group::getPercentOfPlayersWithScoreInBounds(int score, int lowerLevel, int higherLevel, double *players)
{
    //searching the closest levels that exist in group that are in the range specified
    int low_level = levels.searchFromAbove(lowerLevel);
    int high_level = levels.searchFromBelow(higherLevel);
    if(low_level < 0 || high_level < 0)
    { //no levels in the group match the request
        *players = 0;
        return S_FAILURE;
    }
    //getting the score-ish rank of both levels
    int score_low_rank = levels.rankAtScore(low_level, score);
    int score_high_rank = levels.rankAtScore(high_level, score);

    int sum_low_rank = levels.sumRank(low_level);
    int sum_high_rank = levels.sumRank(high_level);

    int num_of_players_mentioned = score_high_rank - score_low_rank + levels.getSizeAt(low_level, score);
    int sum_of_players = sum_high_rank - sum_low_rank + levels.getSumSize(low_level);
    
    if(sum_of_players == 0 || num_of_players == 0)
    {//no players in this level interval
        *players = 0;
        return S_FAILURE;
    }

    //no players fits the purpose..
    *players = ((double)(num_of_players_mentioned * 100) / sum_of_players);
    return S_SUCCESS;
}

//assumes m > 0
Status Group::averageHighestPlayerLevelByGroup(int m, double *avgLevel)
{
    if (m > num_of_players)
    {
        *avgLevel = 0;
        return S_FAILURE;
    }
    //searching for m highest leveled players in the group
    //this is the lower level:
    int lower_m_level = levels.sumSelectFromAbove(num_of_players - m);
    int sum_rank_lower = levels.sumRank(lower_m_level);
    int highest_level = levels.getHighestLevel();
    //this is the remainder of players to take from lower_m_level:
    int additional_players = sum_rank_lower - (num_of_players - m);
    //now calculating the product of sum * level for each level
    int product = levels.prodRank(highest_level) - levels.prodRank(lower_m_level) + additional_players * lower_m_level;
    *avgLevel = ((double)product / m);
    return S_SUCCESS;
}

//creating an array out of the group tree
void Group::GroupToArray(RNode<Level_ptr> **level_array)
{
    int i = 0;
    for (RTree<Level_ptr>::const_iterator it = levels.begin(); it != levels.end(); ++it)
    {
        level_array[i] = it.getNode();
        i++;
    }
}

void Group::ArrayToGroup(RNode<Level_ptr> **level_array, int size, int sum_of_players) //c'tor by array
{
    levels.remove(0);
    //assuming allocation of Group already happend
    levels.ArrayToTree(level_array, 0, size);
    num_of_players = sum_of_players;
}
