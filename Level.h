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

typedef int Id;
class Level
{
private:
    HashTable<Player_ptr> players; // players is a hashTable: key = id of players, data = pointer Players at this level from the group
    int level;
public:
    Level() : players() {}
    Level(const Level &level) = delete;
    ~Level() = default;
    bool isEmpty() const;
    bool isPlayerExist(Id id) const;
    //Player_ptr getPlayerPtr(Id id);
    //assumes the player is "legal":
    Status addPlayer(Player_ptr player);
    Status removePlayer(Id id);
    int getSizeOfLevel() const; //number of players in this level
    int getLevel() const; //returns what level is this
};
typedef shared_ptr<Level> Level_ptr;
#endif // LEVEL_H_