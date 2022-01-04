#include "Game.h"

bool Game::isInRange(int range, int number) const
{
    return number >= 1 && number <= range;
}

Status Game::MergeGroups(int GroupID1, int GroupID2)
{
    if(!isInRange(num_of_groups, GroupID1) || !isInRange(num_of_groups, GroupID2))
    {
        return S_INVALID_INPUT;
    }
    try {
        groups.Union(groups.Find(GroupID1), groups.Find(GroupID2));
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return S_ALLOCATION_ERROR;
    }
    return S_SUCCESS;
}

Status Game::AddPlayer(int PlayerID, int GroupID, int score)
{
    if (!isInRange(num_of_groups, GroupID) || PlayerID <= 0) // player exists already or group doesn't
    {
        return S_INVALID_INPUT;
    }
    if(players.isPlayerExist(PlayerID))
    {
        return S_FAILURE;
    }
    try
    {
        // we are using T& Tree<T>::getData(int key_to_find)
        // only when we know that Tree<T>.isExist(key_to_find) is true!
        int group_current_id = groups.Find(GroupID);
        Player_ptr player = make_shared<Player>(PlayerID, 0, group_current_id);

        // Adding to players (table)
        players.addPlayer(player);

        // Adding to levels (tree)
        levels.addPlayer(PlayerID, player);

        // Adding to groups (union)
        groups.getData(group_current_id)->addPlayer(PlayerID, player);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return S_ALLOCATION_ERROR;
    }
    return S_SUCCESS;
}

Status Game::RemovePlayer(int PlayerID)
{
    if(PlayerID <= 0)
    {
        return S_INVALID_INPUT;
    }
    if(!players.isPlayerExist(PlayerID))
    {
        return S_FAILURE;
    }
    Player_ptr player = players.getPlayerPtr();
    int group_curr_id = player->getGroup();

    // Removing from players table
    players.removePlayer(PlayerID);

    // Removing from levels tree
    levels.removePlayer(PlayerID, player);

    // Removing from groups union
    groups.getData(group_curr_id)->removePlayer(PlayerID, player);

    return S_SUCCESS;
}

Status Game::increasePlayerIDLevel(int PlayerID, int LevelIncrease)
{
    if (LevelIncrease <= 0 || PlayerID <= 0) // player exists already or group doesn't
    {
        return S_INVALID_INPUT;
    }
    if(!players.isPlayerExist(PlayerID))
    {
        return S_FAILURE;
    }
    try
    {
        Player_ptr player = players.getPlayerPtr(PlayerID);
        int group_id = player->getGroup();
        int level = player->getLevel();

        // removing the player from groups and levels
        groups.getData(group_id)->removePlayer(PlayerID, player);
        levels.removePlayer(PlayerID, player);

        // updating player's info
        player->setLevel(level + LevelIncrease);

        // re-inserting the player
        groups.getData(group_id)->addPlayer(PlayerID, player);
        levels.addPlayer(PlayerID, player);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return S_ALLOCATION_ERROR;
    }
    return S_SUCCESS;
}

Status Game::changePlayerIDScore(int PlayerID, int NewScore)
{
    if(PlayerID <= 0 || isInRange(scale, NewScore))
    {
        return S_INVALID_INPUT;
    }
    if(!players.isPlayerExist(PlayerID))
    {
        return S_FAILURE;
    }
    try
    {
        Player_ptr player = players.getPlayerPtr(PlayerID);
        int group_id = player->getGroup();
        int level = player->getLevel();

        // removing the player from groups and levels
        groups.getData(group_id)->removePlayer(PlayerID, player);
        levels.removePlayer(PlayerID, player);

        // updating player's info
        player->setScore(NewScore);

        // re-inserting the player
        groups.getData(group_id)->addPlayer(PlayerID, player);
        levels.addPlayer(PlayerID, player);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return S_ALLOCATION_ERROR;
    }
    return S_SUCCESS;
}

Status getPercentOfPlayersWithScoreInBounds(int GroupID, int score, int lowerLevel, int higherLevel, double *players);
Status averageHighestPlayerLevelByGroup(int GroupID, int m, double *avgLevel);
Status getPlayersBound(void *DS, int GroupID, int score, int m, int *LowerBoundPlayers, int *HigherBoundPlayers);
