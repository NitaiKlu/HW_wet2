#ifndef LEVEL_H_
#define LEVEL_H_

#include <iostream>
#include "HashTable.h"
#include "Player.h"

using std::cout;
using std::endl;
using std::string;

typedef enum
{
    S_SUCCESS = 0,
    S_FAILURE = -1,
    S_ALLOCATION_ERROR = -2,
    S_INVALID_INPUT = -3
} Status;
class Level;
typedef int Id;
typedef shared_ptr<Level> Level_ptr;
class Level
{
private:
    HashTable<Player_ptr> players; // players is a hashTable: key = id of players, data = pointer Players at this level from the group
    int level;
    Player_ptr getPlayerAt(int index) const;
public:
    Level(int level) : level(level), players() {}
    Level(const Level &level) = delete;
    ~Level() = default;
    bool isEmpty() const;
    bool isPlayerExist(Id id) const;
    Player_ptr getPlayerPtr(Id id);
    //assumes the player is "legal":
    Status addPlayer(Player_ptr player);
    Status removePlayer(Id id);
    int getSizeOfLevel() const; //number of players in this level
    int getSizeOfTable() const; //size of the hashTable
    int getLevel() const; //returns what level is this
    void addAllPlayers(Level_ptr level1, Level_ptr level2); //add all players in level1 and level2 to myself
};
#endif // LEVEL_H_