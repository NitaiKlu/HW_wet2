#include "Player.h"

int Player::getLevel() const
{
    return level;
}

int Player::getGroup() const
{
    return group;
}

int Player::getId() const
{
    return id;
}

void Player::setLevel(int new_level)
{
    level = new_level;
}

void Player::setGroup(int new_group)
{
    group = new_group;
}

void Player::printPlayer() const
{
    cout << "PLAYER:" << endl;
    cout << "id = " << id << endl;
    cout << "level = " << level << endl;
    cout << "group = " << group << endl;
}

bool Player::operator==(const Player &compare) const
{
    return id == compare.id;
}

bool Player::operator!=(const Player &compare) const
{
    return !(*this == compare);
}
