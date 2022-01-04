#ifndef PLAYER_H_
#define PLAYER_H_

#include <iostream>

using std::cout;
using std::endl;
using std::shared_ptr;
class Player;
typedef shared_ptr<Player> Player_ptr;

class Player
{
private:
    int id;
    int level;
    int group;

public:
    Player(int id, int level, int group) : id(id), level(level), group(group) {}
    Player(const Player &copy) = default;
    ~Player() = default;
    int getId() const;
    int getLevel() const;
    int getGroup() const;
    void setGroup(int new_group);
    void setLevel(int new_level);
    void printPlayer() const;
    bool operator==(const Player &compare) const;
    bool operator!=(const Player &compare) const;
};
#endif // PLAYER_H_