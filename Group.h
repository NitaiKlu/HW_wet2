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
    Group(int id, int scale) : id(id), scale(scale), levels(scale), num_of_players(0) {}
    Group(const Group &group) = delete;
    ~Group() = default;
    bool isLevelExist(int level) const;
    bool isEmpty() const;
    Level_ptr getLevelPtr(int level);
    int getNumOfPlayers() const;
    Status addPlayer(Id id, Player_ptr player);
    void addLevel(Level_ptr new_level); //dangerous func, only used once in the code!
    Status removePlayer(Id id, Player_ptr player);
    Status getPercentOfPlayersWithScoreInBounds(int score, int lowerLevel, int higherLevel, double *players);
    Status averageHighestPlayerLevelByGroup(int m, double *avgLevel);
};

#endif //GROUP_H_
