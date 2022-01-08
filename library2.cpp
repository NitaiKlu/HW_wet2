#include "library2.h"
#include "Game.h"

void *Init(int k, int scale)
{
    if (k <= 0 || scale <= 0 || scale > 200)
    {
        return NULL;
    }
    Game *DS = new Game(k, scale);
    return (void *)DS;
}

StatusType MergeGroups(void *DS, int GroupID1, int GroupID2)
{
    if (!DS || GroupID1 <= 0 || GroupID2 <= 0)
    {
        return (StatusType)INVALID_INPUT;
    }
    return (StatusType)((Game *)DS)->MergeGroups(GroupID1, GroupID2);
}

StatusType AddPlayer(void *DS, int PlayerID, int GroupID, int score)
{
    if (!DS || GroupID <= 0 || score <= 0 || PlayerID <= 0)
    {
        return (StatusType)INVALID_INPUT;
    }
    return (StatusType)((Game *)DS)->AddPlayer(PlayerID, GroupID, score);
}

StatusType RemovePlayer(void *DS, int PlayerID)
{
    if (!DS || PlayerID <= 0)
    {
        return (StatusType)INVALID_INPUT;
    }
    return (StatusType)((Game *)DS)->RemovePlayer(PlayerID);
}

StatusType IncreasePlayerIDLevel(void *DS, int PlayerID, int LevelIncrease)
{
    if (!DS || PlayerID <= 0 || LevelIncrease <= 0)
    {
        return (StatusType)INVALID_INPUT;
    }
    return (StatusType)((Game *)DS)->increasePlayerIDLevel(PlayerID, LevelIncrease);
}

StatusType ChangePlayerIDScore(void *DS, int PlayerID, int NewScore)
{
    if (!DS || PlayerID <= 0 || NewScore <= 0)
    {
        return (StatusType)INVALID_INPUT;
    }
    return (StatusType)((Game *)DS)->changePlayerIDScore(PlayerID, NewScore);
}

StatusType GetPercentOfPlayersWithScoreInBounds(void *DS, int GroupID, int score, int lowerLevel, int higherLevel, double *players)
{
    if (!DS || !players || GroupID < 0)
    {
        return (StatusType)INVALID_INPUT;
    }
    return (StatusType)((Game *)DS)->getPercentOfPlayersWithScoreInBounds(GroupID, score, lowerLevel, higherLevel, players);

}

StatusType AverageHighestPlayerLevelByGroup(void *DS, int GroupID, int m, double *level)
{
    if(!DS || !level || GroupID < 0 || m <= 0)
    {
        return (StatusType)INVALID_INPUT;
    }
    return (StatusType)((Game *)DS)->averageHighestPlayerLevelByGroup(GroupID, m, level);
}

//StatusType GetPlayersBound(void *DS, int GroupID, int score, int m,
// int * LowerBoundPlayers, int * HigherBoundPlayers);

void Quit(void **DS)
{
    delete (Game *)*DS;
    *DS = NULL;
}