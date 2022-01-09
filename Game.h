#ifndef GAME_H_
#define GAME_H_
#include <iostream>

#include <cassert>

#include "Group.h"
#include "UnionFind.h"

#define DOES_NOT_EXIST -1

using std::cout;
using std::endl;
using std::string;

/**
 * Level - is a tree of Player pointers. (usually, they are all in the same level)
 * Group - is a tree of Levels, in which all of a Level's players are in the same level in the game.
 *
 */

typedef shared_ptr<Group> Group_ptr;

class Game
{
private:
    int num_of_groups;
    int scale;
    Level players; // all of the players are "in" an abstract level = key-Id (int), data-level of the player(int)
    Group levels;  // all of the players form a group = key-level (int), data-Level(tree of players (id) in this level)
    UnionFind<Group_ptr> groups;
    bool isInRange(int range, int number) const; //returns true if 0 < number <= range
    Group_ptr internalMergeGroups(Group_ptr group1, Group_ptr group2);
    void MergeLevelsToSameGroup(RNode<Level_ptr> *level1, RNode<Level_ptr> *level2, RNode<Level_ptr> *result, Level_ptr result_level); // merging 2 Levels into a large arranged by id Level
    void MergePlayersOfSameLevel(Player_ptr *level1, Player_ptr *level2, int size1, int size2, Player_ptr *result); // merging arrays created in LevelToArray into a large arranged array
    int MergeGroupArrays(RNode<Level_ptr> **group1, RNode<Level_ptr> **group2, int size1, int size2, RNode<Level_ptr> **result);
    void UpdateGroup(Level_ptr *level_array, int size, int new_group); 

public:
    Game(int k, int scale): num_of_groups(k), scale(scale), players(DOES_NOT_EXIST), levels(0, scale), groups(k) {
        for (int i = 1; i <= k; i++)
        {
            Group_ptr group = make_shared<Group>(i, scale);
            groups.MakeSet(i, group);
        }
    } ;
    ~Game() = default;

    // functions from Library1:
    Status MergeGroups(int GroupID1, int GroupID2);
    Status AddPlayer(int PlayerID, int GroupID, int score);
    Status RemovePlayer(int PlayerID);
    Status increasePlayerIDLevel(int PlayerID, int LevelIncrease);
    Status changePlayerIDScore(int PlayerID, int NewScore);
    Status getPercentOfPlayersWithScoreInBounds (int GroupID, int score, int lowerLevel, int higherLevel, double * players);
    Status averageHighestPlayerLevelByGroup(int GroupID, int m, double * avgLevel);
    Status getPlayersBound(void *DS, int GroupID, int score, int m, int * LowerBoundPlayers, int * HigherBoundPlayers);
};
#endif