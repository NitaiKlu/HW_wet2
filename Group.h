#ifndef GROUP_H_
#define GROUP_H_
#include <ostream>
#include "Level.h"
#include "LevelTree.h"

class Group
{
private:
    int id;             //group_id
    const int scale;    //point scale - const
    LevelTree levels;   //rank tree of levels. each rank is an array from 0 to scale
    int num_of_players; //num of players in the group
public:
    Group(int id, int scale) : id(id), scale(scale), levels(scale), num_of_players(0) {
        Level_ptr level_zero = make_shared<Level>(0);
        levels.insert(0, level_zero);
    }
    Group(const Group &group) = delete;
    ~Group() = default;
    bool isLevelExist(int level) const;
    bool isEmpty() const;
    Level_ptr getLevelPtr(int level);
    int getNumOfPlayers() const;
    int getNumOfLevels() const;
    int getId() const;
    int getScale() const;
    Status addPlayer(Id id, Player_ptr player);
    void addLevel(Level_ptr new_level); //dangerous func, only used once in the code!
    Status removePlayer(Id id, Player_ptr player);
    Status getPercentOfPlayersWithScoreInBounds(int score, int lowerLevel, int higherLevel, double *players);
    Status averageHighestPlayerLevelByGroup(int m, double *avgLevel);
    Status getPlayersBound(int score, int m, int *LowerBoundPlayers, int *HigherBoundPlayers);
    void GroupToArray(RNode<Level_ptr> **level_array);
    void ArrayToGroup(RNode<Level_ptr> **level_array, int size, int sum_of_players); //c'tor by array
};

#endif //GROUP_H_
