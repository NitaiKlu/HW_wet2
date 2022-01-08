#include "Game.h"

bool Game::isInRange(int range, int number) const
{
    return number >= 1 && number <= range;
}

//merging level1 and level 2 to result
void Game::MergeLevelsToSameGroup(RNode<Level_ptr> *level1, RNode<Level_ptr> *level2, RNode<Level_ptr> *result) // merging 2 Levels into a large arranged by id Level
{
    /**merge level nodes in 2 tasks:
     * 1. merge the players - transferring players from one level to another
     * 2. merge the sizes array of each node
     * note: no need to update weights - this will happen afterwards when creating a tree
     * */
    //task 1:
    Level_ptr result_level = make_shared<Level>(level1->getKey());
    result = new RNode<Level_ptr>(level1->getNumOfWeights(), level1->getKey(), result_level);
    result_level->addAllPlayers(level1->getData(), level2->getData());
    //task 2:
    //should I begin from i = 0 ? and until where? what weight is this
    for(int i = 0; i < level1->getNumOfWeights() + 2; i++)
    {
        result->addToSize(i, level2->getSizeAt(i) + level2->getSizeAt(i));
    }

}

int Game::MergeGroupArrays(RNode<Level_ptr> **group1, RNode<Level_ptr> **group2, int size1, int size2, RNode<Level_ptr> **result)
{
    // merge sorting
    int i1 = 0, i2 = 0;
    int res = 0;
    while (i1 < size1 && i2 < size2)
    {
        int curr_level1 = group1[i1]->getKey(); 
        int curr_level2 = group2[i2]->getKey();
        if (curr_level1 < curr_level2)
        {
            result[res] = group1[i1];
            i1++;
        }

        else if (curr_level1 > curr_level2)
        {
            result[res] = group2[i2];
            i2++;
        }

        // the levels are equal. need to merge between them
        else
        {
            MergeLevelsToSameGroup(group1[i1], group2[i2], result[res]);
            i1++;
            i2++;
        }
        res++;
    }
    // one of these while is not entered during runtime
    while (i1 < size1)
    {
        result[res] = group1[i1];
        i1++;
        res++;
    }

    while (i2 < size2)
    {
        result[res] = group2[i2];
        i2++;
        res++;
    }
    return res;
}

void Game::internalMergeGroups(Group_ptr group1, Group_ptr group2)
{
    Group_ptr bigger_group, smaller_group;
    //deciding who's bigger
    if(group1->getNumOfPlayers() > group2->getNumOfPlayers()) {
        bigger_group = group1;
        smaller_group = group2;
    }
    else {
        bigger_group = group2;
        smaller_group = group1;
    }
    Group_ptr new_group = make_shared<Group>(bigger_group->getId(), smaller_group->getScale());
    int bigger_size_of_group = bigger_group->getNumOfPlayers();
    int smaller_size_of_group = smaller_group->getNumOfPlayers();
    int result_size = bigger_size_of_group + smaller_size_of_group; // the real result size might be smaller!
    //as we know - each player could be in only one group
    int sum_of_players = bigger_group->getNumOfPlayers() + smaller_group->getNumOfPlayers();

    // creating arrays of Level_ptr o(n + m)
    RNode<Level_ptr> **array1, **array2, **result_array; //arrays of pointer to nodes
    array1 = new RNode<Level_ptr>*[bigger_size_of_group];
    try
    {
        array2 = new RNode<Level_ptr>*[smaller_size_of_group];
    }
    catch(const std::exception& e)
    {
        delete[] array1;
        throw;
    }
    try
    {
        result_array = new RNode<Level_ptr>*[result_size];
    }
    catch(const std::exception& e)
    {
        delete[] array1;
        delete[] array2;
        throw;
    }
    bigger_group->GroupToArray(array1);
    smaller_group->GroupToArray(array2);
    // merging the group arrays we created o(n + m)
    result_size = MergeGroupArrays(array1, array2, bigger_size_of_group, smaller_size_of_group, result_array);
    // making it a group o(n + m)
    new_group->ArrayToGroup(result_array, result_size, sum_of_players);
    delete[] array1;
    delete[] array2;
    delete[] result_array;
}

Status Game::MergeGroups(int GroupID1, int GroupID2)
{
    if (!isInRange(num_of_groups, GroupID1) || !isInRange(num_of_groups, GroupID2))
    {
        return S_INVALID_INPUT;
    }
    try
    {
        if(GroupID1 == GroupID2) //same group
        {
            return S_SUCCESS;
        }
        if(groups.Find(GroupID1) == groups.Find(GroupID2)) //groups were already merged
        {
            return S_SUCCESS;
        }
        //if we reached here, need to merge groups. let's do itttttt
        internalMergeGroups(groups.getData(GroupID1), groups.getData(GroupID2));
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
    if (!isInRange(num_of_groups, GroupID) || PlayerID <= 0 || score > scale) // player exists already or group doesn't
    {
        return S_INVALID_INPUT;
    }
    if (players.isPlayerExist(PlayerID))
    {
        return S_FAILURE;
    }
    try
    {
        // we are using T& Tree<T>::getData(int key_to_find)
        // only when we know that Tree<T>.isExist(key_to_find) is true!
        int group_current_id = groups.Find(GroupID); //o(log*k)
        Player_ptr player = make_shared<Player>(PlayerID, 0, group_current_id, score);

        // Adding to players (table) o(1)
        players.addPlayer(player);

        // Adding to levels (tree) o(1)
        levels.addPlayer(PlayerID, player);

        // Adding to groups (union) o(1)
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
    if (PlayerID <= 0)
    {
        return S_INVALID_INPUT;
    }
    if (!players.isPlayerExist(PlayerID))
    {
        return S_FAILURE;
    }
    Player_ptr player = players.getPlayerPtr(PlayerID);
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
    if (!players.isPlayerExist(PlayerID))
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
    if (PlayerID <= 0 || !isInRange(scale, NewScore))
    {
        return S_INVALID_INPUT;
    }
    if (!players.isPlayerExist(PlayerID))
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

Status Game::getPercentOfPlayersWithScoreInBounds(int GroupID, int score, int lowerLevel, int higherLevel, double *players)
{
    Group_ptr group;
    if (GroupID != 0) //a specific group and not the entire game
    {
        group = groups.getData(GroupID);
        return group->getPercentOfPlayersWithScoreInBounds(score, lowerLevel, higherLevel, players);
    }
    else
    {
        return levels.getPercentOfPlayersWithScoreInBounds(score, lowerLevel, higherLevel, players);
    }
}

Status Game::averageHighestPlayerLevelByGroup(int GroupID, int m, double *avgLevel)
{
    if (!isInRange(num_of_groups, GroupID) || m <= 0) //group or m are illegal
    {
        return S_INVALID_INPUT;
    }
    try
    {
        Group_ptr group;
        if (GroupID != 0) //a specific group and not the entire game
        {
            group = groups.getData(GroupID);
            return group->averageHighestPlayerLevelByGroup(m, avgLevel);
        }
        else
        {
            return levels.averageHighestPlayerLevelByGroup(m, avgLevel);
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return S_ALLOCATION_ERROR;
    }
    
}
//Status getPlayersBound(void *DS, int GroupID, int score, int m, int *LowerBoundPlayers, int *HigherBoundPlayers);
