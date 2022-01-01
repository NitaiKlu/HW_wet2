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

class Player;
typedef int Id;
typedef shared_ptr<Player> Player_ptr;

class Level
{
private:
    HashTable<Player> players; // players is a hashTable: key = id of players, data = pointer Players at this level from the group
    int level;
public:
    Level() : players() {}
    Level(const Level &level) = delete;
    ~Level() = default;
    bool isEmpty();
    bool isPlayerExist(Id id);
    //Player_ptr getPlayerPtr(Id id);
    //assumes the player is "legal"
    Status addPlayer(Player& player);
    Status removePlayer(Id id);
    int getSizeOfLevel(); //number of players in this level
    int getLevel(); //returns what level is this
};

#endif // LEVEL_H_